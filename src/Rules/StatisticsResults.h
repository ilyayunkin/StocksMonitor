#ifndef STATISTICSRESULTS_H
#define STATISTICSRESULTS_H

#include <QString>

#include <vector>
#include "StringTypes.h"

struct StatisticsGroupCounter
{
    QString name;
    float totalSum;

    float totalDerivation;
    float totalDerivationWeek;
    float totalDerivationMonth;
    float totalDerivationYear;

    float totalDerivationPercent;
    float totalDerivationWeekPercent;
    float totalDerivationMonthPercent;
    float totalDerivationYearPercent;

    float fractionPercent;
    std::vector<Ticker> tickers;

    explicit StatisticsGroupCounter(const QString &name)
        : name(name)
        , totalSum(0)
        , totalDerivation(0)
        , totalDerivationWeek(0)
        , totalDerivationMonth(0)
        , totalDerivationYear(0)
        , totalDerivationPercent(0)
        , totalDerivationWeekPercent(0)
        , totalDerivationMonthPercent(0)
        , totalDerivationYearPercent(0)
        , fractionPercent(0)
    {}
};

struct StatisticsCounter
{
    QString name;
    std::vector<StatisticsGroupCounter> list;
    explicit StatisticsCounter(const QString &name)
        : name(name)
    {}
};
typedef std::vector<StatisticsCounter> StatisticsCounterList;

struct Statistics
{
    float totalSum;

    float totalDerivation;
    float totalDerivationWeek;
    float totalDerivationMonth;
    float totalDerivationYear;

    float totalDerivationPercent;
    float totalDerivationWeekPercent;
    float totalDerivationMonthPercent;
    float totalDerivationYearPercent;

    StatisticsCounterList list;
    Statistics()
        : totalSum(0)
        , totalDerivation(0)
        , totalDerivationWeek(0)
        , totalDerivationMonth(0)
        , totalDerivationYear(0)
        , totalDerivationPercent(0)
        , totalDerivationWeekPercent(0)
        , totalDerivationMonthPercent(0)
        , totalDerivationYearPercent(0)
    {}
};


#endif // STATISTICSRESULTS_H
