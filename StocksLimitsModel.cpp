#include "StocksLimitsModel.h"

#include <QBrush>
#include <QTimer>
#include <QMessageBox>

#include <algorithm>
#include <assert.h>

StocksLimitsModel::StocksLimitsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    stockLimits.reserve(100);
    colors.reserve(100);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("db_name.sqlite");

    if (!db.open()) {
        qDebug() << db.lastError().text();
    }else
    {
        {
            QSqlQuery q = executeQuery(
                        "create table if not exists "
                        "Limits (ticker TEXT, name TEXT, base_price REAL)");
        }
        {
            QSqlQuery q = executeQuery("SELECT * FROM Limits");
            QSqlRecord rec = q.record();
            while (q.next()) {
                StockLimit limit{};
                limit.ticker = q.value(rec.indexOf("ticker")).toByteArray();
                limit.name = q.value(rec.indexOf("name")).toByteArray();
                limit.basePrice = q.value(rec.indexOf("base_price")).toFloat();
                stockLimits.push_back(limit);
            }
            {// Delete copies
                std::sort(stockLimits.begin(), stockLimits.end(),
                          [](const StockLimit &lhs, const StockLimit &rhs){return lhs.ticker < rhs.ticker;});
                StockLimitsList::iterator it = stockLimits.begin();
                while((it = std::adjacent_find(it, stockLimits.end()))
                      != stockLimits.end())
                {
                    bool answer =
                    {
                        QMessageBox::question(0,tr("Delete double?"),
                        QString("%1 %2\n%3 %4")
                        .arg(QString(it->ticker))
                        .arg(it->basePrice)
                        .arg(QString((it + 1)->ticker))
                        .arg((it + 1)->basePrice))
                        == QMessageBox::Yes
                    };
                    if(answer)
                    {
                        executeQuery(QString("DELETE FROM Limits "
                                             "WHERE ticker = '%1';")
                                     .arg(QString(it->ticker)));
                        executeQuery(QString("INSERT INTO Limits (ticker, name, base_price) "
                                             "VALUES ('%1', '%2', '%3');")
                                     .arg(QString(it->ticker))
                                     .arg(it->name).arg(it->basePrice));
                        it = stockLimits.erase(it);
                    }else
                    {
                        ++it;
                    }
                }
            }
            for(auto const &limit : stockLimits)
            {
                (void)limit;
                colors.push_back(Color::NO_COLOR);
            }
        }
    }
    {
        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout, this, &StocksLimitsModel::update);
        t->start(5000);
    }
    assert(stockLimits.size() == colors.size());
}

void StocksLimitsModel::update()
{
    bool boundCross = false;
    if(stocksModel != nullptr)
    {
        int i = 0;
        for(auto &limit : stockLimits)
        {
            float price = stocksModel->getStockPrice(limit.ticker);
            if(price != limit.price)
            {
                limit.price = price;
                emit dataChanged(createIndex(i, 0), createIndex(i, COL_COUNT - 1));

                const Color c = colorForDistance(distance(stockLimits[i]));
                if(c < colors[i])
                {
                    boundCross = true;
                }
                colors[i] = c;
            }
            ++i;
        }
    }
    if(boundCross)
    {
        emit boundCrossed();
    }
}

int StocksLimitsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return stockLimits.size();
}

int StocksLimitsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COL_COUNT;
}

Qt::ItemFlags StocksLimitsModel::flags(const QModelIndex & index) const
{    
    Qt::ItemFlags ret =QAbstractTableModel::flags(index) |
            Qt::ItemIsEnabled |
            Qt::ItemIsSelectable;
    if(index.column() == BASE_PRICE)
    {
        ret |= Qt::ItemIsEditable;
    }

    return ret;
}

QVariant StocksLimitsModel::headerData(int section,
                                       Qt::Orientation orientation,
                                       int role) const
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
            case TICKER:
                ret = tr("TICKER");
                break;
            case PRICE:
                ret = tr("Price");
                break;
            case BASE_PRICE:
                ret = tr("Base Price");
                break;
            case DISTANCE:
                ret = tr("Distance");
                break;
            default:
                break;
            }
        }
    }
    return ret;
}
QVariant StocksLimitsModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    int row = index.row();
    int col = index.column();
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if(row < stockLimits.size())
        {
            const StockLimit &stock = stockLimits.at(row);
            switch (col) {
            case NAME:
                ret = stock.name;
                break;
            case TICKER:
                ret = stock.ticker;
                break;
            case PRICE:
                ret = stock.price;
                break;
            case BASE_PRICE:
                ret = stock.basePrice;
                break;
            case DISTANCE:
                ret = (stock.price - stock.basePrice) / stock.basePrice;
                break;
            default:
                break;
            }
        }
    }
    if (role == Qt::BackgroundRole)
    {
        if(row < stockLimits.size())
        {
            ret = brushForColor(colors.at(row));
        }
    }
    return ret;
}

void StocksLimitsModel::setStocksModel(AbstractStocksModel *stocksModel)
{
    this->stocksModel = stocksModel;
}

bool StocksLimitsModel::setData(const QModelIndex &index,
                                const QVariant &value,
                                int role)
{
    assert(stockLimits.size() == colors.size());
    int row = index.row();
    int col = index.column();

    if ((role == Qt::EditRole) && (col == BASE_PRICE) && (row < stockLimits.size()))
    {
        float v = value.toFloat();
        stockLimits[row].basePrice = v;
        colors[row] = colorForDistance(distance(stockLimits[row]));
        {
            executeQuery(QString("UPDATE Limits "
                                 "SET base_price = '%1' "
                                 "WHERE ticker = '%2';")
                         .arg(stockLimits[row].basePrice).arg(QString(stockLimits[row].ticker)));
            emit dataChanged(createIndex(row, 0), createIndex(row, COL_COUNT - 1));
        }
        return true;
    }
    return false;
}

void StocksLimitsModel::addStock(const StockLimit &stockLimit)
{
    auto size = stockLimits.size();
    {
        executeQuery(QString("INSERT INTO Limits (ticker, name, base_price) VALUES ('%1', '%2', '%3');")
                     .arg(QString(stockLimit.ticker)).arg(stockLimit.name).arg(stockLimit.basePrice));
    }

    beginInsertRows(QModelIndex(), size, size);

    stockLimits.push_back(stockLimit);
    colors.push_back(colorForDistance(distance(stockLimit)));

    endInsertRows();
    assert(stockLimits.size() == colors.size());
}

QSqlQuery StocksLimitsModel::executeQuery(const QString &query)
{
    QSqlQuery q = db.exec(query);
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastQuery();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastError();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.record();

    return q;
}


