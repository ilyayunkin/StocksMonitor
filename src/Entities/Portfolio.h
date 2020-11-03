#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "PortfolioEntry.h"
#include "StocksList.h"
#include "CurrencyCounter.h"

struct Portfolio{
    PortfolioEntryList portfolio;
    CurrencyCountersList sum() const;
    size_t size() const;
    PortfolioEntry getPortfolioEntry(const size_t i) const;
    void registerStockSourceInPortfolio(const QString &name,
                                        const stocksListHandler handler);
    auto begin() {return portfolio.begin();}
    auto end() {return portfolio.end();}
    auto rbegin() {return portfolio.rbegin();}
    auto rend() {return portfolio.rend();}
    auto begin()const {return portfolio.begin();}
    auto end()const {return portfolio.end();}
    auto cbegin()const {return portfolio.cbegin();}
    auto cend()const {return portfolio.cend();}
    auto rbegin()const {return portfolio.rbegin();}
    auto rend()const {return portfolio.rend();}
    auto operator[](PortfolioEntryList::size_type i){return portfolio[i];}
    auto operator[](PortfolioEntryList::size_type i)const {return portfolio[i];}
};

#endif // PORTFOLIO_H
