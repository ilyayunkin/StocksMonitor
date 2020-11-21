#ifndef PORTFOLIOENTRY_H
#define PORTFOLIOENTRY_H

#include <vector>

#include "Entities/StocksListHandler.h"
#include "StocksList.h"
#include "StringTypes.h"

struct PortfolioEntry
{
    PluginName plugin;
    StockName name;
    Ticker ticker;
    int quantity;
    float price;
    float derivation;
    float derivationWeek;
    float derivationMonth;
    float derivationYear;
    float sellPrice;
    float sum;
    CurrencyCode currency;
    stocksListHandler handler;

    PortfolioEntry(PluginName plugin, Ticker ticker,
                   int quantity, float sellPrice)
        : plugin(std::move(plugin))
        , ticker(std::move(ticker))
        , quantity(quantity)
        , price(0)
        , derivation(0)
        , derivationWeek(0)
        , derivationMonth(0)
        , derivationYear(0)
        , sellPrice(sellPrice)
        , sum(0)
        , handler(0)
    {};
    PortfolioEntry(PluginName plugin, const Stock &stock,
                   int quantity,
                   CurrencyCode currency,
                   stocksListHandler handler)
        : plugin(std::move(plugin))
        , name(stock.name)
        , ticker(stock.ticker)
        , quantity(quantity)
        , price(stock.price)
        , derivation(stock.derivation)
        , derivationWeek(stock.derivationWeek)
        , derivationMonth(stock.derivationMonth)
        , derivationYear(stock.derivationYear)
        , sellPrice(0)
        , sum(price * quantity)
        , currency(std::move(currency))
        , handler(handler)
    {}
};
typedef std::vector<PortfolioEntry> PortfolioEntryList;

#endif // PORTFOLIOENTRY_H
