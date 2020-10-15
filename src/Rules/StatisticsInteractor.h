#ifndef STATISTICSINTERACTOR_H
#define STATISTICSINTERACTOR_H

#include <QString>

#include "Entities/Statistics.h"
#include "Entities/StocksListHandler.h"
#include "StocksList.h"
#include "CurrencyCounter.h"
#include "StatisticsResults.h"

class AbstractStatisticsConfigDatabase;
class Entities;
class AbstractDialogs;
class AbstractCurrencyConverter;

class StatisticsInteractor
{
    AbstractStatisticsConfigDatabase &db;
    Entities &entities;
    AbstractCurrencyConverter * converter = nullptr;
    AbstractDialogs * dialogs = nullptr;
    CurrencyCountersList getPortfolioSum() const;
    Stock getStockForPortfolioEntry(const size_t i) const;
    Stock getStock(const stocksListHandler handler,
                   const char *const ticker) const;

    bool isItemInCategory(const StatisticsConfigList::iterator category, const char *ticker);

public:
    StatisticsInteractor(AbstractStatisticsConfigDatabase &db,
                         Entities &entities);

    void setConverter(AbstractCurrencyConverter *const converter);
    void setDialogs(AbstractDialogs *const dialogs);

    const StatisticsConfigList &getStatisticsConfig()const;
    bool addStatisticsItem(const QString &category,
                           const QString &group,
                           const StockId &stock);
    bool addStatisticsGroup(const QString &category,
                            const QString &groupName);
    bool addStatisticsCategory(const QString &entryName);
    bool removeStatisticsItem(const QString &category,
                              const QString &group,
                              const char * const ticker);
    bool removeStatisticsGroup(const QString &category,
                               const QString &group);
    bool removeStatisticsCategory(const QString &category);
    Statistics processStatistics() const;
};

#endif // STATISTICSINTERACTOR_H
