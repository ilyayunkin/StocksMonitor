#ifndef ENTITIES_H
#define ENTITIES_H

#include "StockLimit.h"
#include "StocksList.h"
#include "PortfolioEntry.h"

#include <vector>

struct StockListsPair
{
    const QString name;
    StockLimitsList limits;
    StocksList stocks;
    QByteArray time;
    const QByteArray currencyCode;

    StockListsPair(const QString name,
                   const QByteArray currencyCode,
                   StockLimitsList &&limits) :
        name(name), limits(limits), currencyCode(currencyCode){}
};

struct Entities
{
    std::vector<StockListsPair> pairs;
    PortfolioEntryList portfolio;
};

#endif // ENTITIES_H
