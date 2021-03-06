#ifndef BUYREQUESTDATABASE_H
#define BUYREQUESTDATABASE_H

#include "Rules/AbstractBuyRequestDatabase.h"
#include <QtSql>

class BuyRequestDatabase : public AbstractBuyRequestDatabase
{
public:
    explicit BuyRequestDatabase(const PluginName &name);

    // AbstractBuyRequestDatabase interface
    void add(const StockLimit &stockLimit) override;
    void remove(const char *const ticker) override;
    void update(const StockLimit &stockLimit) override;
    StockLimitsList getAll();
private:
    QSqlDatabase db;
    QSqlQuery executeQuery(const QString &query);
};

#endif // BUYREQUESTDATABASE_H
