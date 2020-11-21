#ifndef STOCKLIMIT_H
#define STOCKLIMIT_H

#include <vector>

#include "StocksList.h"
#include "StringTypes.h"

struct StockLimit
{
    StockName name;
    Ticker ticker;
    float price;
    float basePrice;
    StockLimit() = default;
    StockLimit(const Stock &stock, float basePrice) :
        name(stock.name), ticker(stock.ticker), price(stock.price), basePrice(basePrice)
    {}
    bool operator ==(const StockLimit &other)
    {
        return (ticker == other.ticker)
                && (basePrice == other.basePrice);
    }
};
typedef std::vector<StockLimit> StockLimitsList;

#endif // STOCKLIMIT_H
