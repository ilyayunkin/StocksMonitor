#include "Subscriptions.h"
#include "AbstractStocksView.h"

Subscriptions::Subscriptions()
{

}

void Subscriptions::subscribeForBuyRequestChanges(
        const stocksListHandler handler, AbstractStocksView *view)
{
    buyRequestSubscribersMap.insert(SubscribersMap::value_type(handler, view));
}

void Subscriptions::subscribeForPortfolioChanges(AbstractStocksView *view)
{
    portfolioSubscriber = view;
}

void Subscriptions::subscribeForStocksChanges(
        const stocksListHandler handler, AbstractStocksView *view)
{
    stocksSubscribersMap.insert(SubscribersMap::value_type(handler, view));
}

void Subscriptions::updateStocksView(const stocksListHandler handler)
{
    auto subscriberIt = stocksSubscribersMap.find(handler);
    bool found = subscriberIt != stocksSubscribersMap.end();
    if(found)
    {
        subscriberIt->second->stocksUpdated();
    }
}

void Subscriptions::updateBuyRequestView(const stocksListHandler handler)
{
    auto subscriberIt = buyRequestSubscribersMap.find(handler);
    bool found = subscriberIt != buyRequestSubscribersMap.end();
    if(found)
    {
        subscriberIt->second->stocksUpdated();
    }
}

void Subscriptions::updatePortfolioView()
{
    if(portfolioSubscriber)
    {
        portfolioSubscriber->stocksUpdated();
    }
}

void Subscriptions::updatePortfolioView(size_t row)
{
    if(portfolioSubscriber)
    {
        portfolioSubscriber->stocksUpdated(row);
    }
}
