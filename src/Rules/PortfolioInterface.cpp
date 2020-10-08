#include "PortfolioInterface.h"
#include "RulesFasade.h"

PortfolioInterface::PortfolioInterface(RulesFasade * const rules) :
    rules(rules)
{

}

size_t PortfolioInterface::size() const
{
    return rules->getPortfolioSize();
}

PortfolioEntry PortfolioInterface::getPortfolioEntry(const size_t i) const
{
    return rules->getPortfolioEntry(i);
}

Stock PortfolioInterface::getStock(const size_t i) const
{
    return rules->getStockForPortfolioEntry(i);
}

void PortfolioInterface::deletePortfolioEntry(size_t row)
{
    rules->deletePortfolioEntry(row);
}

bool PortfolioInterface::setPortfolioEntryQuantity(size_t row, int quantity)
{
    return rules->setPortfolioEntryQuantity(row, quantity);
}

bool PortfolioInterface::setPortfolioEntryReferencePrice(size_t row, float referencePrice)
{
    return rules->setPortfolioEntryReferencePrice(row, referencePrice);
}

void PortfolioInterface::setView(AbstractStocksView * const view)
{
    this->view = view;
}

void PortfolioInterface::update()
{
    if(view)
    {
        view->stocksUpdated();
    }
}

void PortfolioInterface::update(const size_t row)
{
    if(view)
    {
        view->stocksUpdated(row);
    }
}
