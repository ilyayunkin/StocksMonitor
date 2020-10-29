#ifndef ABSTRACTSTATISTICSCONFIGVIEW_H
#define ABSTRACTSTATISTICSCONFIGVIEW_H

#include "Entities/Statistics.h"
#include "StatisticsConfigIndex.h"

class AbstractStatisticsConfigView
{
public:
    virtual ~AbstractStatisticsConfigView() = default;

    virtual void addItem(const StatisticsConfigItemIndex &index) = 0;
    virtual void addGroup(const StatisticsConfigGroupIndex &index) = 0;
    virtual void addCategory(const StatisticsConfigCategoryIndex &index) = 0;

    virtual void removeItem(const StatisticsConfigItemIndex &index) = 0;
    virtual void removeGroup(const StatisticsConfigGroupIndex &index) = 0;
    virtual void removeCategory(const StatisticsConfigCategoryIndex &index) = 0;
};

#endif // ABSTRACTSTATISTICSCONFIGVIEW_H
