#ifndef BUYREQUESTDATABASE_H
#define BUYREQUESTDATABASE_H

#include "Rules/AbstractBuyRequestDatabase.h"
#include <QtSql>

class BuyRequestDatabase : public AbstractBuyRequestDatabase
{
    QSqlDatabase db;
    QSqlQuery executeQuery(const QString &query);
public:
    explicit BuyRequestDatabase(const QString &name);


    // AbstractBuyRequestDatabase interface
public:
    void add(const StockLimit &stockLimit);
    void update(const StockLimit &stockLimit);
    StockLimitsList getAll();
};

#endif // BUYREQUESTDATABASE_H
