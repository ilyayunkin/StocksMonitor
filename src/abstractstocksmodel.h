#ifndef ABSTRACTSTOCKSMODEL_H
#define ABSTRACTSTOCKSMODEL_H

#include "StocksList.h"

class AbstractStocksModel
{
public:
    virtual void setStocks(StocksList &&stocks) = 0;
    virtual size_t size() const = 0;
    virtual Stock getStock(const QByteArray &ticker) const = 0;
    virtual Stock getStock(const size_t i) const = 0;
    virtual float getStockPrice(const QByteArray &ticker) const = 0;
    virtual ~AbstractStocksModel(){};
};

#endif // ABSTRACTSTOCKSMODEL_H
