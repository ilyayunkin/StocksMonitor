#ifndef STOCKID_H
#define STOCKID_H

#include <vector>

#include "StringTypes.h"

struct StockId
{
    Ticker ticker;
    StockName name;
    StockId(Ticker ticker, StockName name)
        : ticker(std::move(ticker))
        , name(std::move(name))
    {}
};
bool operator==(const StockId &lhs, const StockId &rhs);

typedef std::vector<StockId> StockIdList;

#endif // STOCKID_H
