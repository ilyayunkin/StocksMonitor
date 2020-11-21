#ifndef STATISTICS_H
#define STATISTICS_H

#include <vector>

#include <QString>

#include "StockId.h"

struct StatisticsGroupConfig
{
    QString name;
    StockIdList list;
    explicit StatisticsGroupConfig(const QString &name)
        : name(name)
    {}
    StatisticsGroupConfig(const QString &name, const StockIdList &list)
        : name(name)
        , list(list)
    {}
};

typedef std::vector<StatisticsGroupConfig> StatisticsGroupConfigList;

struct StatisticsCathegoryConfig
{
    QString name;
    StatisticsGroupConfigList list;
    explicit StatisticsCathegoryConfig(const QString &name)
        : name(name)
    {}
    StatisticsCathegoryConfig(const QString &name, const StatisticsGroupConfigList &list)
        : name(name)
        , list(list)
    {}
};

typedef std::vector<StatisticsCathegoryConfig> StatisticsConfigList;

#endif // STATISTICS_H
