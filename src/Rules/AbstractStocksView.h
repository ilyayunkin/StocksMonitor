#ifndef ABSTRACTSTOCKSVIEW_H
#define ABSTRACTSTOCKSVIEW_H

class AbstractStocksView
{
public:
    virtual ~AbstractStocksView() = default;
    virtual void stocksUpdated() = 0;
};

#endif // ABSTRACTSTOCKSVIEW_H
