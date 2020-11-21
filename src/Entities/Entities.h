#ifndef ENTITIES_H
#define ENTITIES_H

#include "StockLimit.h"
#include "StocksList.h"
#include "Portfolio.h"
#include "Statistics.h"
#include <vector>
#include <string>

struct StockListsPair
{
    const QString name;
    StockLimitsList limits;
    StocksList stocks;
    std::string time;
    const std::string currencyCode;

    StockListsPair(const QString name,
                   const std::string currencyCode,
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
    std::string getStocksActualizationTime(const stocksListHandler handler) const;
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
    std::string getUrl(const stocksListHandler handler,
                       const char *const ticker) const;
};

#endif // ENTITIES_H
