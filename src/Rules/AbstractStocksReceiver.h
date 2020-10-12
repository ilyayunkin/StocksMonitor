#ifndef ABSTRACTSTOCKSRECEIVER_H
#define ABSTRACTSTOCKSRECEIVER_H

#include "Entities/StocksListHandler.h"
#include "StocksList.h"

class AbstractStocksReceiver
{
public:
    virtual ~AbstractStocksReceiver() = default;
    virtual void setStocks(const stocksListHandler handler,
                           StocksList &&stocks,
                           const char *const time) = 0;
};

#endif // ABSTRACTSTOCKSRECEIVER_H
