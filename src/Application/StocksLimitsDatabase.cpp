#include "StocksLimitsDatabase.h"

#include <QBrush>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

#include <algorithm>
#include <assert.h>

#include "Presenters/StockHint.h"
#include "Presenters/LimitBackgrounColor.h"
#include "Rules/AbstractStocksModel.h"

StocksLimitsDatabase::StocksLimitsDatabase(QString name, QObject *parent) :
    QObject(parent),
    name(name)
{
    stockLimits.reserve(100);
    colors.reserve(100);
    db = QSqlDatabase::addDatabase("QSQLITE", name);
    db.setDatabaseName(name + ".sqlite");

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
        t = new QTimer;
        QObject::connect(t, &QTimer::timeout, this, &StocksLimitsDatabase::update);
        t->start(5000);
    }
    assert(stockLimits.size() == colors.size());
}

StocksLimitsDatabase::~StocksLimitsDatabase()
{
    delete t;
}

void StocksLimitsDatabase::update()
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

                const Color c = LimitBackgrounColor::colorForDistance(distance(stockLimits[i]));
                if(c < colors[i])
                {
                    boundCross = true;
                    emit crossedLimit(limit);
                }
                colors[i] = c;
                view->stocksUpdated(i);
            }
            ++i;
        }
    }
    if(boundCross)
    {
        emit boundCrossed();
    }
}

size_t StocksLimitsDatabase::size() const
{
    return stockLimits.size();
}

QBrush StocksLimitsDatabase::getBrush(int row) const
{    
    if(row < colors.size())
    {
        return QBrush(LimitBackgrounColor::brushForColor(colors.at(row)));
    }
    return QBrush();
}

StockLimit StocksLimitsDatabase::getLimit(int row) const
{
    if(row < stockLimits.size())
    {
        return stockLimits.at(row);
    }
    return StockLimit();
}

QString StocksLimitsDatabase::getToolTip(int row) const
{
    if(row < stockLimits.size())
    {
        auto stock = stocksModel->getStock(stockLimits.at(row).ticker);
        return StockHint::getHint(stock);
    }
    return QString();
}

void StocksLimitsDatabase::setStocksModel(AbstractStocksModel *stocksModel)
{
    this->stocksModel = stocksModel;
}

bool StocksLimitsDatabase::setReferencePrice(int row, float referencePrice)
{
    if (row < stockLimits.size())
    {
        stockLimits[row].basePrice = referencePrice;
        colors[row] = LimitBackgrounColor::colorForDistance(distance(stockLimits[row]));
        {
            executeQuery(QString("UPDATE Limits "
                                 "SET base_price = '%1' "
                                 "WHERE ticker = '%2';")
                         .arg(stockLimits[row].basePrice).arg(QString(stockLimits[row].ticker)));
            view->stocksUpdated(row);
        }
        return true;
    }
    return false;
}

void StocksLimitsDatabase::setView(AbstractStocksView *view)
{
    this->view = view;
}

void StocksLimitsDatabase::addStock(const StockLimit &stockLimit)
{
    {
        StockLimitsList::iterator it = std::find_if(stockLimits.begin(), stockLimits.end(),
                                                    [&](const StockLimit &l){return l.ticker == stockLimit.ticker;});
        if(it != stockLimits.end())
        {
            bool answer =
            {
                QMessageBox::question(0,tr("Replace?"),
                tr("There already is %1:\n %1 %2")
                .arg(QString(it->ticker))
                .arg(it->basePrice))
                == QMessageBox::Yes
            };
            if(answer)
            {
                *it = stockLimit;
                int row = it - stockLimits.begin();
                view->stocksUpdated();
            }

            return;
        }
    }
    {
        auto size = stockLimits.size();
        {
            executeQuery(QString("INSERT INTO Limits (ticker, name, base_price) VALUES ('%1', '%2', '%3');")
                         .arg(QString(stockLimit.ticker)).arg(stockLimit.name).arg(stockLimit.basePrice));
        }

        stockLimits.push_back(stockLimit);
        colors.push_back(LimitBackgrounColor::colorForDistance(distance(stockLimit)));

        view->stocksUpdated();
        assert(stockLimits.size() == colors.size());
    }
}

QSqlQuery StocksLimitsDatabase::executeQuery(const QString &query)
{
    QSqlQuery q = db.exec(query);
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastQuery();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastError();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.record();

    return q;
}


