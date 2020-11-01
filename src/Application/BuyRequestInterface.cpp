#include "BuyRequestInterface.h"

#include "Entities/Entities.h"
#include "Rules/LoadStocksInteractor.h"
#include "Rules/EditPortfolioInteractor.h"
#include "Rules/EditBuyRequestInteractor.h"
#include "Rules/Subscriptions.h"

BuyRequestInterface::BuyRequestInterface(
        const Entities &entities,
        Subscriptions &subscriptions,
        LoadStocksInteractor &loadStocksInteractor,
        EditPortfolioInteractor &editPortfolioInteractor,
        EditBuyRequestInteractor &editBuyRequestInteractor,
        const stocksListHandler handler)
    : handler(handler)
    , entities(entities)
    , subscriptions(subscriptions)
    , loadStocksInteractor(loadStocksInteractor)
    , editPortfolioInteractor(editPortfolioInteractor)
    , editBuyRequestInteractor(editBuyRequestInteractor)
{
}

size_t BuyRequestInterface::size() const
{
    return entities.getStockBuyRequestsCount(handler);
}

StockLimit BuyRequestInterface::getStockBuyRequest(const size_t i) const
{
    return entities.getStockBuyRequest(handler, i);
}

StockLimit BuyRequestInterface::getStockBuyRequest(const char *const ticker) const
{
    return entities.getStockBuyRequest(handler, ticker);
}

bool BuyRequestInterface::setReferencePrice(size_t row, float referencePrice)
{
    return editBuyRequestInteractor.setReferencePrice(handler, row, referencePrice);
}

void BuyRequestInterface::addToPortfolio(const char *const ticker, const int quantity)
{
    editPortfolioInteractor.addToPortfolio(handler, ticker, quantity);
}

void BuyRequestInterface::subscribeForChanges(AbstractStocksView *view)
{
    subscriptions.subscribeForBuyRequestChanges(handler, view);
}
