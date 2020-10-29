#ifndef STATISTICSCONTROLLER_H
#define STATISTICSCONTROLLER_H

#include <QString>

#include "Entities/Statistics.h"
#include "StatisticsConfigIndex.h"

class StatisticsInteractor;
class AbstractStatisticsView;
class AbstractStatisticsConfigView;

class StatisticsController
{
    StatisticsInteractor &rules;
    AbstractStatisticsView &resultView;
    AbstractStatisticsConfigView *configView = nullptr;
public:
    StatisticsController(StatisticsInteractor &rules,
                         AbstractStatisticsView &resultView);
    void setConfigView(AbstractStatisticsConfigView *configView);

    const StatisticsConfigList &getStatisticsConfig()const;
    bool addItem(const StatisticsConfigItemIndex &index);
    bool addGroup(const StatisticsConfigGroupIndex &index);
    bool addCategory(const StatisticsConfigCategoryIndex &index);

    bool removeItem(const StatisticsConfigItemIndex &index);
    bool removeGroup(const StatisticsConfigGroupIndex &index);
    bool removeCategory(const StatisticsConfigCategoryIndex &index);

    void processStatistics() const;
};

#endif // STATISTICSCONTROLLER_H
