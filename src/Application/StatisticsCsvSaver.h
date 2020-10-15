#ifndef STATISTICSCSVSAVER_H
#define STATISTICSCSVSAVER_H

#include "AbstractStatisticsView.h"

class StatisticsCsvSaver final : public AbstractStatisticsView
{
public:
    StatisticsCsvSaver();
    void show(const Statistics &statistics) override;
};

#endif // STATISTICSCSVSAVER_H
