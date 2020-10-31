#ifndef PORTFOLIOINTERFACE_H
#define PORTFOLIOINTERFACE_H

#include "AbstractStocksView.h"

#include <cstdlib>

#include <QString>

#include "StocksList.h"
#include "Entities/PortfolioEntry.h"
#include "Entities/StocksListHandler.h"

class Entities;
class Subscriptions;
class EditPortfolioInteractor;

class PortfolioInterface
{
public:
    explicit PortfolioInterface(Entities const &entities,
                                Subscriptions &subscriptions,
                                EditPortfolioInteractor &editPortfolioInteractor);
    size_t size() const;
    PortfolioEntry getPortfolioEntry(const size_t i) const;
    Stock getStock(const size_t i) const;
    void deletePortfolioEntry(size_t row);
    bool setPortfolioEntryQuantity(size_t row, int quantity);
    bool setPortfolioEntryReferencePrice(size_t row, float referencePrice);
    void subscribeForChanges(AbstractStocksView *view);
private:
    Entities const &entities;
    Subscriptions &subscriptions;
    EditPortfolioInteractor &editPortfolioInteractor;
};

#endif // PORTFOLIOINTERFACE_H
