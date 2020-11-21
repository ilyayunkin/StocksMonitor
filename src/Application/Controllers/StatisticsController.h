#ifndef STATISTICSCONTROLLER_H
#define STATISTICSCONTROLLER_H

#include "Entities/Statistics.h"
#include "StatisticsConfigIndex.h"

class StatisticsInteractor;
class AbstractStatisticsView;
class AbstractStatisticsConfigView;

class StatisticsController
{
public:
    explicit StatisticsController(StatisticsInteractor &rules);
    void setConfigView(AbstractStatisticsConfigView *configView);

    const StatisticsConfigList &getStatisticsConfig()const;
    bool addItem(const StatisticsConfigItemIndex &index);
    bool addGroup(const StatisticsConfigGroupIndex &index);
    bool addCategory(const StatisticsConfigCategoryIndex &index);

    bool removeItem(const StatisticsConfigItemIndex &index);
    bool removeGroup(const StatisticsConfigGroupIndex &index);
    bool removeCategory(const StatisticsConfigCategoryIndex &index);

private:
    StatisticsInteractor &rules;
    AbstractStatisticsConfigView *configView = nullptr;
};

#endif // STATISTICSCONTROLLER_H
