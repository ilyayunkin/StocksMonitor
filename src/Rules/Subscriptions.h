#ifndef SUBSCRIBES_H
#define SUBSCRIBES_H

#include "Entities/StocksListHandler.h"

#include <map>
class AbstractStocksView;

class Subscriptions
{
public:
    Subscriptions();
    void subscribeForStocksChanges(const stocksListHandler handler,
                                   AbstractStocksView *view);
    void subscribeForBuyRequestChanges(const stocksListHandler handler,
                                   AbstractStocksView *view);
    void subscribeForPortfolioChanges(AbstractStocksView *view);
    void updateStocksView(const stocksListHandler handler);
    void updateBuyRequestView(const stocksListHandler handler);
    void updatePortfolioView();
    void updatePortfolioView(size_t row);
private:
    typedef std::map<const stocksListHandler, AbstractStocksView *> SubscribersMap;
    SubscribersMap stocksSubscribersMap;
    SubscribersMap buyRequestSubscribersMap;
    AbstractStocksView *portfolioSubscriber = nullptr;
};

#endif // SUBSCRIBES_H
