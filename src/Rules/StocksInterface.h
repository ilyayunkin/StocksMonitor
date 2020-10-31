     #ifndef STOCKSINTERFACE_H
#define STOCKSINTERFACE_H

#include "AbstractStocksView.h"

#include <cstdlib>

#include <QString>

#include "StocksList.h"
#include "Entities/StocksListHandler.h"

class Entities;
class LoadStocksInteractor;
class EditBuyRequestInteractor;
class EditPortfolioInteractor;
class Subscriptions;

class StocksInterface
{
public:
    StocksInterface(const Entities &entities,
                    Subscriptions &subscriptions,
                    LoadStocksInteractor &loadStocksInteractor,
                    EditBuyRequestInteractor &editBuyRequestInteractor,
                    EditPortfolioInteractor &editPortfolioInteractor,
                    const stocksListHandler handler);
    size_t size() const;
    Stock getStock(const size_t i) const;
    Stock getStock(const char *const ticker) const;
    std::string getActualizationTime() const;
    void addToPortfolio(const char *const ticker, const int quantity);
    void addLimit(const char *const ticker, float limit);
    void subscribeForChanges(AbstractStocksView *view);
private:
    const stocksListHandler handler;
    const Entities &entities;
    Subscriptions &subscriptions;
    LoadStocksInteractor &loadStocksInteractor;
    EditBuyRequestInteractor &editBuyRequestInteractor;
    EditPortfolioInteractor &editPortfolioInteractor;
};

#endif // STOCKSINTERFACE_H
