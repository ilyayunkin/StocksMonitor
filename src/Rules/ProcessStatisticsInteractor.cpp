#include "ProcessStatisticsInteractor.h"

#include "AbstractCurrencyConverter.h"
#include "StatisticsResults.h"

#include "ExceptionClasses.h"

#include <assert.h>
#include <set>
#include <memory>

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

Statistics ProcessStatisticsInteractor::processStatistics() const
{
    if(portfolio.empty())
        throw EmptyPortfolioException();

    Statistics statistics;
    processCategoies(statistics);
    statistics.totalSum = converter->convert("RUB", portfolio.sum()).list.front().sum;
    {
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
    }

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

namespace  {
auto percentDertivationToTotal(float price, float percent)
{
    return price / (100 + percent) * percent;
}

void inlist(StatisticsGroupCounter *groupCounter, const PortfolioEntry &portfolioEntry,
            const AbstractCurrencyConverter *const converter)
{
    assert(groupCounter);
    assert(converter);
    const auto price = converter->convert(
                "RUB",
                portfolioEntry.currency.data(),
                portfolioEntry.sum);
    groupCounter->totalSum+= price;
    groupCounter->totalDerivation+= percentDertivationToTotal(price, portfolioEntry.derivation);
    groupCounter->totalDerivationWeek+= percentDertivationToTotal(price, portfolioEntry.derivationWeek);
    groupCounter->totalDerivationMonth+= percentDertivationToTotal(price, portfolioEntry.derivationMonth);
    groupCounter->totalDerivationYear+= percentDertivationToTotal(price, portfolioEntry.derivationYear);
    groupCounter->tickers.push_back(portfolioEntry.ticker);
}

struct AbstractCategoryProcessor
{
    virtual void operator()(const PortfolioEntry &portfolioEntry) = 0;
    virtual StatisticsCounter getCounter() = 0;
    virtual ~AbstractCategoryProcessor() = default;
};

struct CathegoryProcessor : AbstractCategoryProcessor
{
    const Portfolio &portfolio;
    AbstractCurrencyConverter *converter;
    const StatisticsCathegoryConfig &entry;
    Statistics &statistics;
    StatisticsCounter gcList;
    StatisticsGroupCounter *unknownCounter;
    CathegoryProcessor(const StatisticsCathegoryConfig &entry,
                       const Portfolio &portfolio,
                       AbstractCurrencyConverter *converter,
                       Statistics &statistics)
        : portfolio(portfolio)
        , converter(converter)
        , entry(entry)
        , statistics(statistics)
        , gcList(entry.name)
    {
        for(const auto &group : entry.list)
        {
            gcList.list.emplace_back(group.name);
        }
        gcList.list.emplace_back("UNKNOWN");
        unknownCounter = &gcList.list.back();
    }
    StatisticsCounter getCounter() override {return gcList;}
    void operator()(const PortfolioEntry &portfolioEntry) override
    {
        auto groupCounter =[&]{
            auto f = [&portfolioEntry](const auto &group)
            {return std::any_of(group.list.begin(), group.list.end(),
                                [&portfolioEntry](auto const&id){ return portfolioEntry.ticker == id.ticker;});};
            auto group = std::find_if(entry.list.begin(), entry.list.end(), f);
            if(group != entry.list.end())
            {
                auto f = [&group](auto const &gc)
                {return gc.name == group->name;};
                auto groupCounter =
                        std::find_if(gcList.list.begin(), gcList.list.end(), f);
                assert(groupCounter != gcList.list.end());
                return &*groupCounter;
            }
            return unknownCounter;
        }();
        inlist(groupCounter, portfolioEntry, converter);
    }
};
struct CurrencyProcessor : AbstractCategoryProcessor
{
    const Portfolio &portfolio;
    AbstractCurrencyConverter *converter;
    Statistics &statistics;
    StatisticsCounter gcList;
    CurrencyProcessor(const Portfolio &portfolio,
                      AbstractCurrencyConverter *converter,
                      Statistics &statistics)
        : portfolio(portfolio)
        , converter(converter)
        , statistics(statistics)
        , gcList("Currency")
    {
        const auto &portfolioSum = portfolio.sum();
        for(const auto &currency : portfolioSum.list)
        {
            gcList.list.emplace_back(currency.currency.data());
        }
    }
    StatisticsCounter getCounter() override {return gcList;}
    void operator()(const PortfolioEntry &portfolioEntry) override
    {
        auto f = [&portfolioEntry](auto const &gc)
        {return gc.name == portfolioEntry.currency.data();};
        auto groupCounter =
                std::find_if(gcList.list.begin(), gcList.list.end(), f);
        assert(groupCounter != gcList.list.end());
        inlist(&*groupCounter, portfolioEntry, converter);
    }
};
struct TrendProcessor : AbstractCategoryProcessor
{
    const Portfolio &portfolio;
    AbstractCurrencyConverter *converter;
    Statistics &statistics;
    StatisticsCounter gcList;
    StatisticsGroupCounter *positiveGroup;
    StatisticsGroupCounter *negativeGroup;
    TrendProcessor(const Portfolio &portfolio,
                   AbstractCurrencyConverter *converter,
                   Statistics &statistics)
        : portfolio(portfolio)
        , converter(converter)
        , statistics(statistics)
        , gcList("Trend")
    {
        gcList.list.reserve(2);
        gcList.list.emplace_back("Negative");
        this->negativeGroup = &gcList.list.back();
        gcList.list.emplace_back("Positive");
        this->positiveGroup = &gcList.list.back();
    }
    StatisticsCounter getCounter() override {return gcList;}
    void operator()(const PortfolioEntry &portfolioEntry) override
    {
        auto *groupCounter = (portfolioEntry.derivation > 0)
                ? positiveGroup
                : negativeGroup;
        inlist(groupCounter, portfolioEntry, converter);
    }
};
struct PluginsProcessor : AbstractCategoryProcessor
{
    const Portfolio &portfolio;
    AbstractCurrencyConverter *converter;
    Statistics &statistics;
    StatisticsCounter gcList;
    PluginsProcessor(const Portfolio &portfolio,
                     AbstractCurrencyConverter *converter,
                     Statistics &statistics)
        : portfolio(portfolio)
        , converter(converter)
        , statistics(statistics)
        , gcList("Plugins")
    {
        std::set<PluginName> plugins;
        std::transform(portfolio.portfolio.begin(),
                       portfolio.portfolio.end(),
                       std::inserter(plugins, plugins.begin()),
                       [](const auto &entry){return entry.plugin;});
        for(const auto &plugin : plugins)
        {
            gcList.list.emplace_back(plugin);
        }
    }
    StatisticsCounter getCounter() override {return gcList;}
    void operator()(const PortfolioEntry &portfolioEntry) override
    {
        auto f = [&portfolioEntry](auto const &gc)
        {return gc.name == portfolioEntry.plugin;};
        auto groupCounter =
                std::find_if(gcList.list.begin(), gcList.list.end(), f);
        assert(groupCounter != gcList.list.end());
        inlist(&*groupCounter, portfolioEntry, converter);
    }
};
}

void ProcessStatisticsInteractor::processCategoies(Statistics &statistics) const
{
    std::vector<std::unique_ptr<AbstractCategoryProcessor>> processors;
    processors.reserve(3 + statisticsConfig.size());
    for(const auto &entry : statisticsConfig)
        processors.emplace_back(new CathegoryProcessor(entry, portfolio, converter, statistics));

    processors.emplace_back(new CurrencyProcessor(portfolio, converter, statistics));
    processors.emplace_back(new TrendProcessor(portfolio, converter, statistics));
    processors.emplace_back(new PluginsProcessor(portfolio, converter, statistics));

    for(const auto &portfolioEntry : portfolio.portfolio)
    {
        std::for_each(processors.begin(), processors.end(),
                      [&](const auto &f){(*f)(portfolioEntry);});

        const auto price = converter->convert(
                    "RUB",
                    portfolioEntry.currency.data(),
                    portfolioEntry.sum);
        statistics.totalDerivation+= percentDertivationToTotal(price, portfolioEntry.derivation);
        statistics.totalDerivationWeek+= percentDertivationToTotal(price, portfolioEntry.derivationWeek);
        statistics.totalDerivationMonth+= percentDertivationToTotal(price, portfolioEntry.derivationMonth);
        statistics.totalDerivationYear+= percentDertivationToTotal(price, portfolioEntry.derivationYear);
    }
    std::for_each(processors.begin(), processors.end(),
                  [&](const auto &f){statistics.list.push_back((*f).getCounter());});
}
