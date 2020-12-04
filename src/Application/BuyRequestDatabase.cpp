#include "BuyRequestDatabase.h"

#include "ExceptionClasses.h"

#include <QMessageBox>

QSqlQuery BuyRequestDatabase::executeQuery(const QString &query)
{
    QSqlQuery q = db.exec(query);
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastQuery();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastError();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.record();

    return q;
}

BuyRequestDatabase::BuyRequestDatabase(const PluginName &name)
    : db(QSqlDatabase::addDatabase("QSQLITE", name))
{    
    db.setDatabaseName(name + ".sqlite");

    if (!db.open()) {
        qDebug() << db.lastError().text();
        throw CantOpenDatabaseException();
    }
    executeQuery(
                "create table if not exists "
                "Limits (ticker TEXT, name TEXT, base_price REAL)");
}

void BuyRequestDatabase::add(const StockLimit &stockLimit)
{
    executeQuery(QString("INSERT INTO Limits (ticker, name, base_price) VALUES ('%1', '%2', '%3');")
                 .arg(QString(stockLimit.ticker.data())).arg(stockLimit.name).arg(stockLimit.basePrice));
}

void BuyRequestDatabase::remove(const char * const ticker)
{
    executeQuery(QString("DELETE FROM "
                         "Limits "
                         "WHERE "
                         "ticker = '%1'"
                         ";")
                 .arg(ticker));
}

void BuyRequestDatabase::update(const StockLimit &stockLimit)
{
    executeQuery(QString("UPDATE Limits "
                         "SET base_price = '%1' "
                         "WHERE ticker = '%2';")
                 .arg(stockLimit.basePrice).arg(QString(stockLimit.ticker.data())));
}

StockLimitsList BuyRequestDatabase::getAll()
{
    StockLimitsList stockLimits;
    {
        QSqlQuery q = executeQuery("SELECT * FROM Limits");
        QSqlRecord rec = q.record();
        while (q.next()) {
            StockLimit limit{};
            limit.ticker = q.value(rec.indexOf("ticker")).toByteArray().data();
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
                    QMessageBox::question(0, QObject::tr("Delete double?"),
                    QString("%1 %2\n%3 %4")
                    .arg(QString(it->ticker.data()))
                    .arg(it->basePrice)
                    .arg(QString((it + 1)->ticker.data()))
                    .arg((it + 1)->basePrice))
                    == QMessageBox::Yes
                };
                if(answer)
                {
                    executeQuery(QString("DELETE FROM Limits "
                                         "WHERE ticker = '%1';")
                                 .arg(QString(it->ticker.data())));
                    executeQuery(QString("INSERT INTO Limits (ticker, name, base_price) "
                                         "VALUES ('%1', '%2', '%3');")
                                 .arg(QString(it->ticker.data()))
                                 .arg(it->name).arg(it->basePrice));
                    it = stockLimits.erase(it);
                }else
                {
                    ++it;
                }
            }
        }
    }
    return stockLimits;
}
