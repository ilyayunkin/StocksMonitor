#ifndef STATISTICSRESULTS_H
#define STATISTICSRESULTS_H

#include <QString>

#include <vector>
#include <list>
#include <string>

struct StatisticsGroupCounter
{
    QString name;
    float sum;
    float fractionPercent;
    std::vector<std::string> tickers;

    StatisticsGroupCounter(const QString &name)
        : name(name)
        , sum(0)
        , fractionPercent(0)
    {}
};

struct StatisticsCounter
{
    QString name;
    std::vector<StatisticsGroupCounter> list;
    StatisticsCounter(const QString &name)
        : name(name)
    {}
};
typedef std::list<StatisticsCounter> StatisticsCounterList;

struct Statistics
{
    float totalSum;
    float totalDerivation;
    float totalDerivationWeek;
    float totalDerivationMonth;
    float totalDerivationYear;
    StatisticsCounterList list;
    Statistics()
        : totalSum(0)
        , totalDerivation(0)
        , totalDerivationWeek(0)
        , totalDerivationMonth(0)
        , totalDerivationYear(0)
    {}
};


#endif // STATISTICSRESULTS_H
