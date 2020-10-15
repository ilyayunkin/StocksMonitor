#ifndef ABSTRACTSTATISTICSVIEW_H
#define ABSTRACTSTATISTICSVIEW_H

#include "Rules/StatisticsResults.h"

class AbstractStatisticsView
{
public:
    ~AbstractStatisticsView() = default;
    virtual void show(const Statistics &statistics) = 0;
};

#endif // ABSTRACTSTATISTICSVIEW_H
