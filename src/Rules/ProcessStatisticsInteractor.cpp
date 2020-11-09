#include "ProcessStatisticsInteractor.h"

#include "AbstractCurrencyConverter.h"
#include "StatisticsResults.h"

#include "ExceptionClasses.h"

#include <assert.h>
#include <set>
#include <functional>

ProcessStatisticsInteractor::ProcessStatisticsInteractor(
        const Portfolio &entities,
        const StatisticsConfigList &statisticsConfig)
    : portfolio(entities)
    , statisticsConfig(statisticsConfig)
{
}

void ProcessStatisticsInteractor::setConverter(AbstractCurrencyConverter * const converter)
{
    this->converter = converter;
}
namespace  {
struct CathegoryProcessor
{
    const Portfolio &portfolio;
    AbstractCurrencyConverter *converter;
    const StatisticsCathegoryConfig &entry;
    StatisticsCounter *gcList;
    StatisticsGroupCounter *unknownCounter;
    CathegoryProcessor(const StatisticsCathegoryConfig &entry,
                       const Portfolio &portfolio,
                       AbstractCurrencyConverter *converter,
                       Statistics &statistics)
        : portfolio(portfolio)
        , converter(converter)
        , entry(entry)
    {
        {
            StatisticsCounter gcList(entry.name);
            for(const auto &group : entry.list)
            {
                StatisticsGroupCounter groupCounter(group.name);
                gcList.list.push_back(groupCounter);
            }
            StatisticsGroupCounter unknownCounter("UNKNOWN");
            gcList.list.push_back(unknownCounter);
            statistics.list.push_back(gcList);
        }
        this->gcList  = &statistics.list.back();
        this->unknownCounter = &gcList->list.back();
    }
    void operator()(const PortfolioEntry &portfolioEntry)
    {
        auto f = [&portfolioEntry](const auto &group)
        {return std::any_of(group.list.begin(), group.list.end(),
                            [&portfolioEntry](auto const&id){ return portfolioEntry.ticker == id.ticker;});};
        auto group = std::find_if(entry.list.begin(), entry.list.end(), f);
        const auto price = converter->convert(
                    "RUB",
                    portfolioEntry.currency.data(),
                    portfolioEntry.sum);
        if(group != entry.list.end())
        {
            auto f = [&group](auto const &gc)
            {return gc.name == group->name;};
            auto groupCounter =
                    std::find_if(gcList->list.begin(), gcList->list.end(), f);
            assert(groupCounter != gcList->list.end());
            groupCounter->totalSum+= price;
            groupCounter->totalDerivation+= price / (100 + portfolioEntry.derivation) * portfolioEntry.derivation;
            groupCounter->totalDerivationWeek+= price / (100 + portfolioEntry.derivationWeek) * portfolioEntry.derivationWeek;
            groupCounter->totalDerivationMonth+= price / (100 + portfolioEntry.derivationMonth) * portfolioEntry.derivationMonth;
            groupCounter->totalDerivationYear+= price / (100 + portfolioEntry.derivationYear) * portfolioEntry.derivationYear;
            groupCounter->tickers.push_back(portfolioEntry.ticker);
        }else
        {
            unknownCounter->totalSum+= price;
            unknownCounter->totalDerivation+= price / (100 + portfolioEntry.derivation) * portfolioEntry.derivation;
            unknownCounter->totalDerivationWeek+= price / (100 + portfolioEntry.derivationWeek) * portfolioEntry.derivationWeek;
            unknownCounter->totalDerivationMonth+= price / (100 + portfolioEntry.derivationMonth) * portfolioEntry.derivationMonth;
            unknownCounter->totalDerivationYear+= price / (100 + portfolioEntry.derivationYear) * portfolioEntry.derivationYear;
            unknownCounter->tickers.push_back(portfolioEntry.ticker);
        }
    }
};
struct CurrencyProcessor
{
    const Portfolio &portfolio;
    AbstractCurrencyConverter *converter;
    StatisticsCounter *gcList;
    CurrencyProcessor(const Portfolio &portfolio,
                      AbstractCurrencyConverter *converter,
                      Statistics &statistics)
        : portfolio(portfolio)
        , converter(converter)
    {
        const auto &portfolioSum = portfolio.sum();
        {
            StatisticsCounter gcList("Currency");
            for(const auto &currency : portfolioSum.list)
            {
                StatisticsGroupCounter groupCounter(currency.currency.data());
                gcList.list.push_back(groupCounter);
            }
            statistics.list.push_back(gcList);
        }
        this->gcList  = &statistics.list.back();
    }
    void operator()(const PortfolioEntry &portfolioEntry)
    {
        auto f = [&portfolioEntry](auto const &gc)
        {return gc.name == portfolioEntry.currency.data();};
        auto groupCounter =
                std::find_if(gcList->list.begin(), gcList->list.end(), f);
        assert(groupCounter != gcList->list.end());
        const auto price = converter->convert(
                    "RUB",
                    portfolioEntry.currency.data(),
                    portfolioEntry.sum);
        groupCounter->totalSum+= price;
        groupCounter->totalDerivation+= price / (100 + portfolioEntry.derivation) * portfolioEntry.derivation;
        groupCounter->totalDerivationWeek+= price / (100 + portfolioEntry.derivationWeek) * portfolioEntry.derivationWeek;
        groupCounter->totalDerivationMonth+= price / (100 + portfolioEntry.derivationMonth) * portfolioEntry.derivationMonth;
        groupCounter->totalDerivationYear+= price / (100 + portfolioEntry.derivationYear) * portfolioEntry.derivationYear;
        groupCounter->tickers.push_back(portfolioEntry.ticker);
    }
};
struct TrendProcessor
{
    const Portfolio &portfolio;
    AbstractCurrencyConverter *converter;
    StatisticsCounter *gcList;
    StatisticsGroupCounter *positiveGroup;
    StatisticsGroupCounter *negativeGroup;
    TrendProcessor(const Portfolio &portfolio,
                   AbstractCurrencyConverter *converter,
                   Statistics &statistics)
        : portfolio(portfolio)
        , converter(converter)
    {
        {
            StatisticsCounter gcList("Trend");

            statistics.list.push_back(gcList);
        }
        this->gcList  = &statistics.list.back();
        StatisticsGroupCounter positiveGroup("Positive");
        StatisticsGroupCounter negativeGroup("Negative");
        gcList->list.reserve(2);
        gcList->list.push_back(positiveGroup);
        this->positiveGroup = &gcList->list.back();
        gcList->list.push_back(negativeGroup);
        this->negativeGroup = &gcList->list.back();
    }
    void operator()(const PortfolioEntry &portfolioEntry)
    {
        const auto price = converter->convert(
                    "RUB",
                    portfolioEntry.currency.data(),
                    portfolioEntry.sum);
        if(portfolioEntry.derivation > 0)
        {
            positiveGroup->totalSum+= price;
            positiveGroup->totalDerivation+= price / (100 + portfolioEntry.derivation) * portfolioEntry.derivation;
            positiveGroup->totalDerivationWeek+= price / (100 + portfolioEntry.derivationWeek) * portfolioEntry.derivationWeek;
            positiveGroup->totalDerivationMonth+= price / (100 + portfolioEntry.derivationMonth) * portfolioEntry.derivationMonth;
            positiveGroup->totalDerivationYear+= price / (100 + portfolioEntry.derivationYear) * portfolioEntry.derivationYear;
            positiveGroup->tickers.push_back(portfolioEntry.ticker);
        }else
        {
            negativeGroup->totalSum+= price;
            negativeGroup->totalDerivation+= price / (100 + portfolioEntry.derivation) * portfolioEntry.derivation;
            negativeGroup->totalDerivationWeek+= price / (100 + portfolioEntry.derivationWeek) * portfolioEntry.derivationWeek;
            negativeGroup->totalDerivationMonth+= price / (100 + portfolioEntry.derivationMonth) * portfolioEntry.derivationMonth;
            negativeGroup->totalDerivationYear+= price / (100 + portfolioEntry.derivationYear) * portfolioEntry.derivationYear;
            negativeGroup->tickers.push_back(portfolioEntry.ticker);
        }
    }
};
struct PluginsProcessor
{
    const Portfolio &portfolio;
    AbstractCurrencyConverter *converter;
    StatisticsCounter *gcList;
    PluginsProcessor(const Portfolio &portfolio,
                     AbstractCurrencyConverter *converter,
                     Statistics &statistics)
        : portfolio(portfolio)
        , converter(converter)
    {
        {
            StatisticsCounter gcList("Plugins");

            statistics.list.push_back(gcList);
        }
        gcList  = &statistics.list.back();

        std::set<QString> plugins;
        std::transform(portfolio.portfolio.begin(),
                       portfolio.portfolio.end(),
                       std::inserter(plugins, plugins.begin()),
                       [](const auto &entry){return entry.plugin;});
        for(const auto &plugin : plugins)
        {
            StatisticsGroupCounter groupCounter(plugin);
            gcList->list.push_back(groupCounter);
        }
    }
    void operator()(const PortfolioEntry &portfolioEntry)
    {
        auto f = [&portfolioEntry](auto const &gc)
        {return gc.name == portfolioEntry.plugin;};
        auto groupCounter =
                std::find_if(gcList->list.begin(), gcList->list.end(), f);
        assert(groupCounter != gcList->list.end());
        const auto price = converter->convert(
                    "RUB",
                    portfolioEntry.currency.data(),
                    portfolioEntry.sum);
        groupCounter->totalSum+= price;
        groupCounter->totalDerivation+= price / (100 + portfolioEntry.derivation) * portfolioEntry.derivation;
        groupCounter->totalDerivationWeek+= price / (100 + portfolioEntry.derivationWeek) * portfolioEntry.derivationWeek;
        groupCounter->totalDerivationMonth+= price / (100 + portfolioEntry.derivationMonth) * portfolioEntry.derivationMonth;
        groupCounter->totalDerivationYear+= price / (100 + portfolioEntry.derivationYear) * portfolioEntry.derivationYear;
        groupCounter->tickers.push_back(portfolioEntry.ticker);
    }
};
}

Statistics ProcessStatisticsInteractor::processStatistics() const
{
    if(portfolio.empty())
        throw EmptyPortfolioException();

    Statistics statistics;
    {
        std::vector<std::function<void(const PortfolioEntry &portfolioEntry)>> processors;
        processors.reserve(3 + statisticsConfig.size());
        for(const auto &entry : statisticsConfig)
            processors.emplace_back(CathegoryProcessor(entry, portfolio, converter, statistics));

        processors.emplace_back(CurrencyProcessor(portfolio, converter, statistics));
        processors.emplace_back(TrendProcessor(portfolio, converter, statistics));
        processors.emplace_back(PluginsProcessor(portfolio, converter, statistics));

        for(const auto &portfolioEntry : portfolio.portfolio)
        {
            std::for_each(processors.begin(), processors.end(),[&](auto &f){f(portfolioEntry);});
            if(portfolioEntry.derivation >= -1)
            {
                const auto price = converter->convert(
                            "RUB",
                            portfolioEntry.currency.data(),
                            portfolioEntry.sum);
                statistics.totalDerivation+= price / (100 + portfolioEntry.derivation) * portfolioEntry.derivation;
                statistics.totalDerivationWeek+= price / (100 + portfolioEntry.derivationWeek) * portfolioEntry.derivationWeek;
                statistics.totalDerivationMonth+= price / (100 + portfolioEntry.derivationMonth) * portfolioEntry.derivationMonth;
                statistics.totalDerivationYear+= price / (100 + portfolioEntry.derivationYear) * portfolioEntry.derivationYear;
            }
        }
    }
    statistics.totalSum = converter->convert("RUB", portfolio.sum()).list.front().sum;
    const auto dayAgo = (statistics.totalSum - statistics.totalDerivation) ;
    const auto weekAgo = (statistics.totalSum - statistics.totalDerivationWeek);
    const auto monthAgo = (statistics.totalSum - statistics.totalDerivationMonth);
    const auto yearAgo = (statistics.totalSum - statistics.totalDerivationYear);
    statistics.totalDerivationPercent = (dayAgo > 0)
            ? (statistics.totalDerivation / dayAgo * 100)
            : 0;
    statistics.totalDerivationWeekPercent = (weekAgo > 0)
            ? (statistics.totalDerivationWeek / weekAgo * 100)
            : 0;
    statistics.totalDerivationMonthPercent = (monthAgo > 0)
            ? (statistics.totalDerivationMonth / monthAgo * 100)
            : 0;
    statistics.totalDerivationYearPercent = (yearAgo > 0)
            ? (statistics.totalDerivationYear / yearAgo * 100)
            : 0;

    for(auto &entry : statistics.list)
    {
        for(auto &group : entry.list)
        {
            auto dayAgo = (group.totalSum - group.totalDerivation) ;
            auto weekAgo = (group.totalSum - group.totalDerivationWeek);
            auto monthAgo = (group.totalSum - group.totalDerivationMonth);
            auto yearAgo = (group.totalSum - group.totalDerivationYear);
            group.totalDerivationPercent = (dayAgo > 0)
                    ? (group.totalDerivation / dayAgo * 100)
                    : 0;
            group.totalDerivationWeekPercent = (weekAgo > 0)
                    ? (group.totalDerivationWeek / weekAgo * 100)
                    : 0;
            group.totalDerivationMonthPercent = (monthAgo > 0)
                    ? (group.totalDerivationMonth / monthAgo * 100)
                    : 0;
            group.totalDerivationYearPercent = (yearAgo > 0)
                    ? (group.totalDerivationYear / yearAgo * 100)
                    : 0;
            group.fractionPercent = group.totalSum
                    / statistics.totalSum
                    * 100;
        }
    }
    return statistics;
}
