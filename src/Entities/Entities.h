#ifndef ENTITIES_H
#define ENTITIES_H

#include "StockLimit.h"
#include "StocksList.h"
#include "PortfolioEntry.h"
#include "Statistics.h"
#include "CurrencyCounter.h"
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
                   const std::string &currencyCode,
                   StockLimitsList &&limits) :
        name(name), limits(limits), currencyCode(currencyCode){}
};

struct Entities
{
    std::vector<StockListsPair> pairs;
    PortfolioEntryList portfolio;
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

    size_t getPortfolioSize() const;
    PortfolioEntry getPortfolioEntry(const size_t i) const;
    Stock getStockForPortfolioEntry(const size_t i) const;
    void registerStockSourceInPortfolio(const QString &name,
                                        const stocksListHandler handler);

    CurrencyCountersList getPortfolioSum() const;
};

#endif // ENTITIES_H
