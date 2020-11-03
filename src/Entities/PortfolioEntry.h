#ifndef PORTFOLIOENTRY_H
#define PORTFOLIOENTRY_H

#include <vector>
#include <string>

#include <QString>

#include "Entities/StocksListHandler.h"
#include "StocksList.h"

struct PortfolioEntry
{
    QString plugin;
    QString name;
    std::string ticker;
    int quantity;
    float price;
    float derivation;
    float derivationWeek;
    float derivationMonth;
    float derivationYear;
    float sellPrice;
    float sum;
    std::string currency;
    stocksListHandler handler;

    PortfolioEntry(QString plugin, std::string ticker,
                   int quantity, float sellPrice)
        : plugin(plugin)
        , ticker(ticker)
        , quantity(quantity)
        , sellPrice(sellPrice)
    {};
    PortfolioEntry(QString plugin, Stock stock,
                   int quantity, std::string currency,
                   stocksListHandler handler)
        : plugin(plugin)
        , name(stock.name)
        , ticker(stock.ticker)
        , quantity(quantity)
        , price(stock.price)
        , derivation(stock.derivation)
        , derivationWeek(stock.derivationWeek)
        , derivationMonth(stock.derivationMonth)
        , derivationYear(stock.derivationYear)
        , sum(price * quantity)
        , currency(currency)
        , handler(handler)
    {}
};
typedef std::vector<PortfolioEntry> PortfolioEntryList;

#endif // PORTFOLIOENTRY_H
