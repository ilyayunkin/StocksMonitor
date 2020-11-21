#include "Portfolio.h"


size_t Portfolio::size() const
{
    return portfolio.size();
}

PortfolioEntry Portfolio::getPortfolioEntry(const size_t i) const
{
    return portfolio.at(i);
}

void Portfolio::registerStockSourceInPortfolio(const PluginName &name, const stocksListHandler handler)
{
    for(auto &e : portfolio)
    {
        if(e.plugin == name)
        {
            e.handler = handler;
        }
    }
}

CurrencyCountersList Portfolio::sum() const
{
    CurrencyCountersList counters;
    for(auto &e : portfolio)
    {
        counters.add(e.currency.data(), e.price * e.quantity);
    }

    return counters;
}
