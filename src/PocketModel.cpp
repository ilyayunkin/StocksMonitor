#include "PocketModel.h"

#include <QMessageBox>

#include <QDebug>

#include <QTimer>

#include "ExceptionClasses.h"
#include "Presenters/StockHint.h"

namespace  {
QString tableName = "Pocket";
}

QSqlQuery PocketModel::executeQuery(const QString &query)
{

    QSqlQuery q = db.exec(query);
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastQuery();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastError();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.record();

    return q;
}

void PocketModel::update()
{
    CurrencyCountersList counters;
    for(auto &e : entries)
    {
        Stock stock = e.model->getStock(e.ticker);
        auto cross = e.sellPrice > 0
                && e.price < e.sellPrice
                && stock.price >= e.sellPrice;
        e.price = stock.price;
        e.sum = e.price * e.quantity;
        if(e.name.isEmpty())
        {
            e.name = stock.name;
        }

        counters.add(e.currency, e.sum);
        if(cross)
        {
            emit boundCrossed();
            emit crossedLimit(e);
        }
    }
    emit dataChanged(createIndex(0, 0),
                     createIndex(entries.size() - 1, COL_COUNT - 1));
    currencyCounters = counters;
}

PocketModel::PocketModel(ModelsReferenceList &models, QObject *parent) :
    QAbstractTableModel(parent),
    models(models)
{
    entries.reserve(100);
    db = QSqlDatabase::addDatabase("QSQLITE", "pocket");
    db.setDatabaseName("pocket.sqlite");
    if (!db.open()) {
        qDebug() << __PRETTY_FUNCTION__ << db.lastError().text();
    }else
    {
        {
            QSqlQuery q = executeQuery(
                        QString("create table if not exists "
                                "%1 (plugin TEXT, ticker TEXT, quantity INTEGER,"
                                " sell_price REAL)")
                        .arg(tableName));
        }
        {
            QSqlQuery q = executeQuery(QString("SELECT * FROM %1").arg(tableName));
            QSqlRecord rec = q.record();
            while (q.next()) {
                auto ticker = q.value(rec.indexOf("ticker")).toByteArray();
                auto plugin = q.value(rec.indexOf("plugin")).toByteArray();
                auto quantity = q.value(rec.indexOf("quantity")).toInt();
                auto sellPrice = q.value(rec.indexOf("sell_price")).toDouble();

                ModelsReferenceList::iterator modelsIt =
                        std::find_if(models.begin(), models.end(),
                                     [&](const ModelsReference &ref){return ref.stocksModel->pluginName() == plugin;});
                if(modelsIt != models.end())
                {
                    StocksModel *stockModel = modelsIt->stocksModel.get();
                    Stock stock = stockModel->getStock(ticker);

                    PortfolioEntry newEntry{plugin, stock.name, ticker, quantity,
                                stock.price, sellPrice, stock.price * quantity,
                                stockModel->currencyCode(), stockModel};
                    entries.push_back(newEntry);
                }
            }
        }
    }
    {
        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout, this, &PocketModel::update);
        t->start(5000);
    }
}

int PocketModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return entries.size();
}

int PocketModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COL_COUNT;
}

Qt::ItemFlags PocketModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags ret =QAbstractTableModel::flags(index) |
            Qt::ItemIsEnabled |
            Qt::ItemIsSelectable;
    if(index.column() == QUANTITY ||
            index.column() == SELL_PRICE)
    {
        ret |= Qt::ItemIsEditable;
    }

    return ret;
}

QVariant PocketModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant ret;
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section) {
            case NAME:
                ret = tr("Name");
                break;
            case QUANTITY:
                ret = tr("Quantity");
                break;
            case TICKER:
                ret = tr("TICKER");
                break;
            case PRICE_BASE_CURRENCY:
                ret = tr("Price");
                break;
            case CURRENCY:
                ret = tr("Currency");
                break;
            case SUM:
                ret = tr("Sum");
                break;
            case SELL_PRICE:
                ret = tr("Sell price");
                break;
            default:
                break;
            }
        }
    }
    return ret;
}

QVariant PocketModel::data(const QModelIndex &index, int role) const
{

    QVariant ret;
    int row = index.row();
    int col = index.column();
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if(row < entries.size())
        {
            const PortfolioEntry &entry = entries.at(row);
            switch (col) {
            case NAME:
                ret = entry.name;
                break;
            case TICKER:
                ret = entry.ticker;
                break;
            case QUANTITY:
                ret = entry.quantity;
                break;
            case PRICE_BASE_CURRENCY:
                ret = entry.price;
                break;
            case CURRENCY:
                ret = entry.currency;
                break;
            case SUM:
                ret = entry.sum;
                break;
            case SELL_PRICE:
                ret = entry.sellPrice;
                break;
            default:
                break;
            }
        }
    }
    if (role == Qt::ToolTipRole)
    {
        if(row < entries.size())
        {
            const PortfolioEntry &entry = entries.at(row);
            auto it = std::find_if(models.begin(), models.end(),
                                   [entry](const ModelsReference &ref){return ref.stocksModel->pluginName() == entry.plugin;});
            if(it != models.end())
            {
                auto stock = it->stocksModel->getStock(entry.ticker);
                ret = StockHint::getHint(stock);
            }
        }
    }
    if (role == Qt::BackgroundRole)
    {
        if(row < entries.size())
        {
            const PortfolioEntry &entry = entries.at(row);
            if(entry.sellPrice > 0
                    && entry.price >= entry.sellPrice)
            {
                ret = QBrush(Qt::red);
            }
        }
    }
    return ret;
}

bool PocketModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int col = index.column();

    if ((role == Qt::EditRole) && (row < entries.size()))
    {
        if(col == QUANTITY)
        {
            int quantity = value.toInt();
            if(quantity > 0)
            {
                PortfolioEntry &entry = entries[row];
                executeQuery(QString("UPDATE %1 "
                                     "SET quantity = '%2' "
                                     "WHERE ticker = '%3';")
                             .arg(tableName)
                             .arg(quantity)
                             .arg(QString(entry.ticker)));
                entry.quantity = quantity;
                entry.sum = quantity * entry.price;
                emit dataChanged(createIndex(row, 0),
                                 createIndex(row, COL_COUNT - 1));
            }else
            {
                const PortfolioEntry entry = entries[row];
                bool answer =
                        QMessageBox::question(0,
                                              tr("Delete?"),
                                              tr("You've selected 0 items:\n %1")
                                              .arg(QString(entry.ticker)))
                        == QMessageBox::Yes;
                if(answer)
                {
                    beginRemoveRows(QModelIndex(), row, row);
                    executeQuery(QString("DELETE FROM %1 "
                                         "WHERE ticker = '%2';")
                                 .arg(tableName)
                                 .arg(QString(entry.ticker)));
                    std::remove_if(entries.begin(), entries.end(),
                                   [&entry](PortfolioEntry &e){return entry.ticker == e.ticker;});
                    endRemoveRows();
                }
            }
            return true;
        }else if(col == SELL_PRICE)
        {
            double sellPrice = value.toDouble();
            PortfolioEntry &entry = entries[row];
            executeQuery(QString("UPDATE %1 "
                                 "SET sell_price = '%2' "
                                 "WHERE ticker = '%3';")
                         .arg(tableName)
                         .arg(sellPrice)
                         .arg(QString(entry.ticker)));
            entry.sellPrice = sellPrice;
            emit dataChanged(createIndex(row, 0),
                             createIndex(row, COL_COUNT - 1));

        }
    }
    return false;
}

void PocketModel::addStock(QString plugin, QByteArray ticker, int quantity)
{
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ ;
    {
        PortfolioEntryList::iterator entryIt =
                std::find_if(entries.begin(), entries.end(),
                             [&](const PortfolioEntry &l){return l.ticker == ticker;});
        if(entryIt != entries.end())
        {
            bool answer =
                    QMessageBox::question(0,
                                          tr("Add?"),
                                          tr("There already is %1:\n %1")
                                          .arg(QString(entryIt->ticker)))
                    == QMessageBox::Yes;
            if(answer)
            {
                entryIt->quantity+= quantity;
                int row = entryIt - entries.begin();
                emit dataChanged(createIndex(row, 0),
                                 createIndex(row, COL_COUNT - 1));
            }

            return;
        }
    }
    {
        ModelsReferenceList::iterator modelsIt =
                std::find_if(models.begin(), models.end(),
                             [&](const ModelsReference &ref){return ref.stocksModel->pluginName() == plugin;});
        if(modelsIt != models.end())
        {
            auto size = entries.size();
            {
                executeQuery(QString("INSERT INTO %1 "
                                     "(plugin, ticker, quantity, sell_price) "
                                     "VALUES ('%2', '%3', '%4', 0);")
                             .arg(tableName)
                             .arg(plugin)
                             .arg(QString(ticker))
                             .arg(quantity));
            }
            StocksModel *stockModel = modelsIt->stocksModel.get();
            Stock stock = stockModel->getStock(ticker);

            PortfolioEntry newEntry{plugin, stock.name, ticker, quantity,
                        stock.price, 0.0, stock.price * quantity,
                        stockModel->currencyCode(), stockModel};

            {
                beginInsertRows(QModelIndex(), size, size);
                entries.push_back(newEntry);

                endInsertRows();
            }
        }else
        {
            throw NoSuchPluginException();
        }
    }
}

CurrencyCountersList PocketModel::sum() const
{
    return currencyCounters;
}
