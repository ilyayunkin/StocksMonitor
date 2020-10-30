     #ifndef STOCKSINTERFACE_H
#define STOCKSINTERFACE_H

#include "AbstractStocksView.h"

#include <cstdlib>

#include <QString>

#include "StocksList.h"
#include "Entities/StocksListHandler.h"

class RulesFasade;

class StocksInterface
{
public:
    StocksInterface(RulesFasade * const rules,
                    const stocksListHandler handler);
    size_t size() const;
    Stock getStock(const size_t i) const;
    Stock getStock(const char *const ticker) const;
    std::string getActualizationTime() const;
    void setView(AbstractStocksView *const view);
    void addToPortfolio(const char *const ticker, const int quantity);
    void addLimit(const char *const ticker, float limit);
    void update();
private:
    const stocksListHandler handler;
    RulesFasade *const rules;
    AbstractStocksView *view = nullptr;
};

#endif // STOCKSINTERFACE_H
