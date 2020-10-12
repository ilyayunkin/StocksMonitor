#include "BuyRequestInterface.h"

#include "RulesFasade.h"

BuyRequestInterface::BuyRequestInterface(RulesFasade *const rules,
                                 const stocksListHandler handler) :
    handler(handler),
    rules(rules)
{

}

size_t BuyRequestInterface::size() const
{
    return rules->getStockBuyRequestsCount(handler);
}

StockLimit BuyRequestInterface::getStockBuyRequest(const size_t i) const
{
    return rules->getStockBuyRequest(handler, i);
}

StockLimit BuyRequestInterface::getStockBuyRequest(const char *const ticker) const
{
    return rules->getStockBuyRequest(handler, ticker);
}

bool BuyRequestInterface::setReferencePrice(size_t row, float referencePrice)
{
    return rules->setReferencePrice(handler, row, referencePrice);
}

void BuyRequestInterface::setView(AbstractStocksView * const view)
{
    this->view = view;
}

void BuyRequestInterface::addToPortfolio(const char *const ticker, const int quantity)
{
    rules->addToPortfolio(handler, ticker, quantity);
}

void BuyRequestInterface::update()
{
    if(view)
    {
        view->stocksUpdated();
    }
}
