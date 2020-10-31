#ifndef BUYREQUESTINTERFACE_H
#define BUYREQUESTINTERFACE_H

#include "AbstractStocksView.h"

#include <cstdlib>

#include <QString>

#include "Entities/StockLimit.h"
#include "Entities/StocksListHandler.h"

class Entities;
class LoadStocksInteractor;
class EditPortfolioInteractor;
class EditBuyRequestInteractor;
class Subscriptions;

class BuyRequestInterface
{
public:
    BuyRequestInterface(const Entities &entities,
                        Subscriptions &subscriptions,
                        LoadStocksInteractor &loadStocksInteractor,
                        EditPortfolioInteractor &editPortfolioInteractor,
                        EditBuyRequestInteractor &editBuyRequestInteractor,
                        const stocksListHandler handler);
    size_t size() const;
    StockLimit getStockBuyRequest(const size_t i) const;
    StockLimit getStockBuyRequest(const char *const ticker) const;
    bool setReferencePrice(size_t row, float referencePrice);
    void addToPortfolio(const char *const ticker, const int quantity);
    void subscribeForChanges(AbstractStocksView *view);
private:
    const stocksListHandler handler;
    const Entities &entities;
    Subscriptions &subscriptions;
    LoadStocksInteractor &loadStocksInteractor;
    EditPortfolioInteractor &editPortfolioInteractor;
    EditBuyRequestInteractor &editBuyRequestInteractor;
};
#endif // BUYREQUESTINTERFACE_H
