#ifndef ENTITIES_H
#define ENTITIES_H

#include "StockLimit.h"
#include "StocksList.h"
#include "PortfolioEntry.h"

#include <vector>

#include "Rules/AbstractBuyRequestDatabase.h"

struct StockListsPair
{
    const QString name;
    StockLimitsList limits;
    StocksList stocks;
    QByteArray time;
    const QByteArray currencyCode;
    AbstractBuyRequestDatabase *const db;

    StockListsPair(const QString name,
                   AbstractBuyRequestDatabase *const db,
                   const QByteArray currencyCode,
                   StockLimitsList &&limits) :
        name(name), limits(limits), currencyCode(currencyCode), db(db){}
};

struct Entities
{
    std::vector<StockListsPair> pairs;
    PortfolioEntryList portfolio;
};

#endif // ENTITIES_H
