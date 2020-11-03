#ifndef STATISTICSINTERACTOR_H
#define STATISTICSINTERACTOR_H

#include <QString>

#include "Entities/Statistics.h"
#include "Entities/StocksListHandler.h"
#include "StocksList.h"

class AbstractStatisticsConfigDatabase;
class Entities;
class AbstractDialogs;

class StatisticsInteractor
{
public:
    StatisticsInteractor(AbstractStatisticsConfigDatabase &db,
                         Entities &entities);

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
private:
    AbstractStatisticsConfigDatabase &db;
    Entities &entities;
    AbstractDialogs * dialogs = nullptr;

    bool isItemInCategory(const StatisticsConfigList::iterator category, const char *ticker);
};

#endif // STATISTICSINTERACTOR_H
