#include "StocksInterface.h"

#include "Entities/Entities.h"
#include "Rules/LoadStocksInteractor.h"
#include "Rules/EditBuyRequestInteractor.h"
#include "Rules/EditPortfolioInteractor.h"
#include "Rules/Subscriptions.h"

#include <QDebug>

StocksInterface::StocksInterface(const Entities &entities,
                                 Subscriptions &subscriptions,
                                 LoadStocksInteractor &loadStocksInteractor,
                                 EditBuyRequestInteractor &editBuyRequestInteractor,
                                 EditPortfolioInteractor &editPortfolioInteractor,
                                 const stocksListHandler handler)
    : handler(handler)
    , entities(entities)
    , subscriptions(subscriptions)
    , loadStocksInteractor(loadStocksInteractor)
    , editBuyRequestInteractor(editBuyRequestInteractor)
    , editPortfolioInteractor(editPortfolioInteractor)
{    
}

size_t StocksInterface::size() const
{
    return entities.getStocksCount(handler);
}

Stock StocksInterface::getStock(const size_t i) const
{
    return entities.getStock(handler, i);
}

Stock StocksInterface::getStock(const char *const ticker) const
{
    return entities.getStock(handler, ticker);
}

std::string StocksInterface::getActualizationTime() const
{
    return entities.getStocksActualizationTime(handler);
}

void StocksInterface::addToPortfolio(const char *const ticker, const int quantity)
{
    editPortfolioInteractor.addToPortfolio(handler, ticker, quantity);
}

void StocksInterface::addLimit(const char *const ticker, float limit)
{
    qDebug() << __PRETTY_FUNCTION__ << __LINE__;
    editBuyRequestInteractor.addLimit(handler, ticker, limit);
}

void StocksInterface::subscribeForChanges(AbstractStocksView *view)
{
    subscriptions.subscribeForStocksChanges(handler, view);
}
