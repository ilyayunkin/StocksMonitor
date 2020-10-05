#ifndef ABSTRACTSTOCKSVIEW_H
#define ABSTRACTSTOCKSVIEW_H

#include <stdint.h>

class AbstractStocksView
{
public:
    virtual ~AbstractStocksView() = default;
    virtual void stocksUpdated() = 0;
    virtual void stocksUpdated(size_t row) = 0;
};

#endif // ABSTRACTSTOCKSVIEW_H
