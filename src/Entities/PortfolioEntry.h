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

    PortfolioEntry(const QString &plugin, const std::string &ticker,
                   int quantity, float sellPrice)
        : plugin(plugin)
        , ticker(ticker)
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
    PortfolioEntry(const QString &plugin, const Stock &stock,
                   int quantity,
                   const std::string &currency,
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
        , sellPrice(0)
        , sum(price * quantity)
        , currency(currency)
        , handler(handler)
    {}
};
typedef std::vector<PortfolioEntry> PortfolioEntryList;

#endif // PORTFOLIOENTRY_H
