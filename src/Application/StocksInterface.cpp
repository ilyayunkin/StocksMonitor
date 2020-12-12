#include "StocksInterface.h"

#include "Entities/Entities.h"
#include "Rules/LoadStocksInteractor.h"
#include "Rules/EditBuyRequestInteractor.h"
#include "Rules/EditPortfolioInteractor.h"
#include "Rules/Subscriptions.h"
#include "Rules/AbstractDialogs.h"
#include "Application/AbstractBrowser.h"

#include <QDebug>

StocksInterface::StocksInterface(const Entities &entities,
                                 Subscriptions &subscriptions,
                                 LoadStocksInteractor &loadStocksInteractor,
                                 EditBuyRequestInteractor &editBuyRequestInteractor,
                                 EditPortfolioInteractor &editPortfolioInteractor,
                                 const AbstractDialogs &dialogs,
                                 const AbstractBrowser &browser,
                                 const stocksListHandler handler)
    : handler(handler)
    , dialogs(dialogs)
    , browser(browser)
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

TimeString StocksInterface::getActualizationTime() const
{
    return entities.getStocksActualizationTime(handler);
}

void StocksInterface::addToPortfolio(const char *const ticker, const int quantity)
{
    editPortfolioInteractor.addToPortfolio(handler, ticker, quantity);
}

void StocksInterface::addLimit(const char * const ticker)
{
    Stock stock = getStock(ticker);
    bool ok;
    float basePrice = dialogs.getBuyRequestPrice(stock.name, stock.price, &ok);
    if(ok)
    {
        editBuyRequestInteractor.addLimit(handler, ticker, basePrice);
    }
}

void StocksInterface::subscribeForChanges(AbstractStocksView *view)
{
    subscriptions.subscribeForStocksChanges(handler, view);
}

void StocksInterface::openUrl(const char * const ticker) const
{
    browser.openUrl(entities.getUrl(handler, ticker));
}
