#ifndef STATISTICSCONTROLLER_H
#define STATISTICSCONTROLLER_H

#include <cstdlib>

#include <QString>

#include "Entities/Statistics.h"

class StatisticsInteractor;
class AbstractStatisticsView;

class StatisticsController
{
    StatisticsInteractor &rules;
    AbstractStatisticsView &view;
public:
    StatisticsController(StatisticsInteractor &rules,
                         AbstractStatisticsView &view);
    const StatisticsConfigList &getStatisticsConfig()const;
    bool addItem(const QString &entry,
                 const QString &group,
                 const StockId &stock);
    bool addGroup(const QString &entry,
                  const QString &groupName);
    bool addCategory(const QString &entryName);

    bool removeItem(const QString &entry,
                    const QString &group,
                    const char *const ticker);
    bool removeGroup(const QString &entry,
                     const QString &group);
    bool removeCategory(const QString &entry);

    void processStatistics() const;
};

#endif // STATISTICSCONTROLLER_H
