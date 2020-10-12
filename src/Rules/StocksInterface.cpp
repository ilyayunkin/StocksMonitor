#include "StocksInterface.h"

#include "RulesFasade.h"

#include <QDebug>

StocksInterface::StocksInterface(RulesFasade *const rules,
                                 const stocksListHandler handler) :
    handler(handler),
    rules(rules)
{

}

size_t StocksInterface::size() const
{
    return rules->getStocksCount(handler);
}

Stock StocksInterface::getStock(const size_t i) const
{
    return rules->getStock(handler, i);
}

Stock StocksInterface::getStock(const char *const ticker) const
{
    return rules->getStock(handler, ticker);
}

QByteArray StocksInterface::getActualizationTime() const
{
    return rules->getStocksActualizationTime(handler);
}

void StocksInterface::setView(AbstractStocksView * const view)
{
    this->view = view;
}

void StocksInterface::addToPortfolio(const char *const ticker, const int quantity)
{
    rules->addToPortfolio(handler, ticker, quantity);
}

void StocksInterface::addLimit(const char *const ticker, float limit)
{
    qDebug() << __PRETTY_FUNCTION__ << __LINE__;
    rules->addLimit(handler, ticker, limit);
}

void StocksInterface::update()
{
    if(view)
    {
        view->stocksUpdated();
    }
}
