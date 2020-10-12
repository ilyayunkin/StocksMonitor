#ifndef PORTFOLIOINTERFACE_H
#define PORTFOLIOINTERFACE_H

#include "AbstractStocksView.h"

#include <cstdlib>

#include <QString>

#include "StocksList.h"
#include "Entities/PortfolioEntry.h"
#include "Entities/StocksListHandler.h"

class RulesFasade;

class PortfolioInterface
{
    RulesFasade *const rules;
    AbstractStocksView *view = nullptr;
public:
    explicit PortfolioInterface(RulesFasade * const rules);
    size_t size() const;
    PortfolioEntry getPortfolioEntry(const size_t i) const;
    Stock getStock(const size_t i) const;
    void deletePortfolioEntry(size_t row);
    bool setPortfolioEntryQuantity(size_t row, int quantity);
    bool setPortfolioEntryReferencePrice(size_t row, float referencePrice);
    void setView(AbstractStocksView *const view);
    void update();
    void update(const size_t row);
};

#endif // PORTFOLIOINTERFACE_H
