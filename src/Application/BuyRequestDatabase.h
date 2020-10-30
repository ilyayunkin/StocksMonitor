#ifndef BUYREQUESTDATABASE_H
#define BUYREQUESTDATABASE_H

#include "Rules/AbstractBuyRequestDatabase.h"
#include <QtSql>

class BuyRequestDatabase : public AbstractBuyRequestDatabase
{
public:
    explicit BuyRequestDatabase(const QString &name);

    // AbstractBuyRequestDatabase interface
    void add(const StockLimit &stockLimit);
    void update(const StockLimit &stockLimit);
    StockLimitsList getAll();
private:
    QSqlDatabase db;
    QSqlQuery executeQuery(const QString &query);
};

#endif // BUYREQUESTDATABASE_H
