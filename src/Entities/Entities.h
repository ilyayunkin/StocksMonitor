#ifndef ENTITIES_H
#define ENTITIES_H

#include "StockLimit.h"
#include "StocksList.h"
#include "Portfolio.h"
#include "Statistics.h"
#include <vector>

struct StockListsPair
{
    const PluginName name;
    StockLimitsList limits;
    StocksList stocks;
    TimeString time;
    const CurrencyCode currencyCode;

    StockListsPair(const PluginName name,
                   const CurrencyCode currencyCode,
                   StockLimitsList limits) :
        name(name), limits(std::move(limits)), currencyCode(std::move(currencyCode)){}
};

struct Entities
{
    std::vector<StockListsPair> pairs;
    Portfolio portfolio;
    StatisticsConfigList statistics;

    float getStockPrice(const stocksListHandler handler,
                        const char *const ticker);
    TimeString getStocksActualizationTime(const stocksListHandler handler) const;
    Stock getStock(const stocksListHandler handler, const size_t i) const;
    Stock getStock(const stocksListHandler handler,
                   const char *const ticker) const;
    size_t getStocksCount(const stocksListHandler handler) const;

    StockLimit getStockBuyRequest(const stocksListHandler handler,
                                  const size_t i) const;
    StockLimit getStockBuyRequest(const stocksListHandler handler,
                                  const char *const ticker) const;
    size_t getStockBuyRequestsCount(const stocksListHandler handler) const;
    Stock getStockForPortfolioEntry(const size_t i) const;
    Url getUrl(const stocksListHandler handler,
                       const char *const ticker) const;
};

#endif // ENTITIES_H
