#include "PortfolioDatabase.h"

#include <QMessageBox>

namespace  {
QString tableName = "Pocket";
}

QSqlQuery PortfolioDatabase::executeQuery(const QString &query)
{
    QSqlQuery q = db.exec(query);
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastQuery();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastError();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.record();

    return q;
}

PortfolioDatabase::PortfolioDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE", "pocket");
    db.setDatabaseName("pocket.sqlite");
    if (!db.open()) {
        qDebug() << __PRETTY_FUNCTION__ << db.lastError().text();
    }else
    {
        QSqlQuery q = executeQuery(
                    QString("create table if not exists "
                            "%1 (plugin TEXT, ticker TEXT, quantity INTEGER,"
                            " sell_price REAL)")
                    .arg(tableName));
    }
}

void PortfolioDatabase::add(const QString &plugin, const QByteArray &ticker, int quantity)
{
    executeQuery(QString("INSERT INTO %1 "
                         "(plugin, ticker, quantity, sell_price) "
                         "VALUES ('%2', '%3', '%4', 0);")
                 .arg(tableName)
                 .arg(plugin)
                 .arg(QString(ticker))
                 .arg(quantity));

}

void PortfolioDatabase::update(const PortfolioEntry &entry)
{
    int quantity = entry.quantity;
    double sellPrice = entry.sellPrice;
    if(quantity > 0)
    {
        executeQuery(QString("UPDATE %1 "
                             "SET quantity = '%2' "
                             "WHERE ticker = '%3';")
                     .arg(tableName)
                     .arg(quantity)
                     .arg(QString(entry.ticker)));
        executeQuery(QString("UPDATE %1 "
                             "SET sell_price = '%2' "
                             "WHERE ticker = '%3';")
                     .arg(tableName)
                     .arg(sellPrice)
                     .arg(QString(entry.ticker)));
    }else
    {
        bool answer =
                QMessageBox::question(0,
                                      QObject::tr("Delete?"),
                                      QObject::tr("You've selected 0 items:\n %1")
                                      .arg(QString(entry.ticker)))
                == QMessageBox::Yes;
        if(answer)
        {
            executeQuery(QString("DELETE FROM %1 "
                                 "WHERE ticker = '%2';")
                         .arg(tableName)
                         .arg(QString(entry.ticker)));
        }
    }
}

void PortfolioDatabase::deleteEntry(const QByteArray &ticker)
{
    executeQuery(QString("DELETE FROM %1 "
                             "WHERE ticker = '%2';")
                     .arg(tableName)
                     .arg(QString(ticker)));
}

PortfolioEntryList PortfolioDatabase::getAll()
{
    PortfolioEntryList entries;
    entries.reserve(100);
    QSqlQuery q = executeQuery(QString("SELECT * FROM %1").arg(tableName));
    QSqlRecord rec = q.record();
    while (q.next()) {
        auto ticker = q.value(rec.indexOf("ticker")).toByteArray();
        auto plugin = q.value(rec.indexOf("plugin")).toString();
        auto quantity = q.value(rec.indexOf("quantity")).toInt();
        auto sellPrice = q.value(rec.indexOf("sell_price")).toFloat();

        //        auto it =
        //                std::find_if(viewInterfaces.begin(), viewInterfaces.end(),
        //                             [&](const ViewInterfacesPair &ref){return ref.name == plugin;});
        //        if(it != viewInterfaces.end())
        //        {
        //            AbstractStocksModel *stockModel = it->stocksModel.get();
        //            Stock stock = stockModel->getStock(ticker);

        //            PortfolioEntry newEntry{plugin, stock.name, ticker, quantity,
        //                        stock.price, sellPrice, stock.price * quantity,
        //                        stockModel->currencyCode(), stockModel};

        PortfolioEntry newEntry{plugin, QString(), ticker, quantity,
                    float(), sellPrice, float(),
                    QByteArray(), stocksListHandler()};
        entries.push_back(newEntry);
        //        }
    }
    return entries;
}
