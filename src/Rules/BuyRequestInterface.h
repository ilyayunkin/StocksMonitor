#ifndef BUYREQUESTINTERFACE_H
#define BUYREQUESTINTERFACE_H

#include "AbstractStocksView.h"

#include <cstdlib>

#include <QString>
#include <QByteArray>

#include "Entities/StockLimit.h"
#include "Entities/StocksListHandler.h"

class RulesFasade;

class BuyRequestInterface
{
    const stocksListHandler handler;
    RulesFasade *const rules;
    AbstractStocksView *view = nullptr;
public:
    BuyRequestInterface(RulesFasade * const rules,
                    const stocksListHandler handler);
    size_t size() const;
    StockLimit getStockBuyRequest(const size_t i) const;
    StockLimit getStockBuyRequest(const QByteArray &ticker) const;
    bool setReferencePrice(size_t row, float referencePrice);
    void setView(AbstractStocksView *const view);
    void addToPortfolio(const QByteArray &ticker, const int quantity);
    void update();
};
#endif // BUYREQUESTINTERFACE_H
