#ifndef ENTITIES_H
#define ENTITIES_H

#include "StockLimit.h"
#include "StocksList.h"
#include "PortfolioEntry.h"
#include "Statistics.h"

#include <vector>
#include <string>

struct StockListsPair
{
    const QString name;
    StockLimitsList limits;
    StocksList stocks;
    std::string time;
    const std::string currencyCode;

    StockListsPair(const QString name,
                   const std::string &currencyCode,
                   StockLimitsList &&limits) :
        name(name), limits(limits), currencyCode(currencyCode){}
};

struct Entities
{
    std::vector<StockListsPair> pairs;
    PortfolioEntryList portfolio;
    StatisticsConfigList statistics;
};

#endif // ENTITIES_H
