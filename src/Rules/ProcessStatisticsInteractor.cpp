#include "ProcessStatisticsInteractor.h"

#include "AbstractCurrencyConverter.h"
#include "Entities/Entities.h"
#include "StatisticsResults.h"

#include <assert.h>
#include <set>

ProcessStatisticsInteractor::ProcessStatisticsInteractor(const Entities &entities)
    : entities(entities)
{
}

void ProcessStatisticsInteractor::setConverter(AbstractCurrencyConverter * const converter)
{
    this->converter = converter;
}

Statistics ProcessStatisticsInteractor::processStatistics() const
{
    Statistics statistics;
    {
        auto const sum = converter->convert("RUB", entities.getPortfolioSum());
        statistics.totalSum = sum.list.front().sum;
    }
    for(const auto &entry : entities.statistics)
    {
        StatisticsCounter gcList(entry.name);
        for(const auto &group : entry.list)
        {
            StatisticsGroupCounter groupCounter(group.name);
            gcList.list.push_back(groupCounter);
        }
        StatisticsGroupCounter unknownCounter("UNKNOWN");
        for(const auto &portfolioEntry : entities.portfolio)
        {
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
                groupCounter->sum+= converter->convert(
                            "RUB",
                            portfolioEntry.currency.data(),
                            portfolioEntry.sum);
                groupCounter->tickers.push_back(portfolioEntry.ticker);
            }else
            {
                unknownCounter.sum+= converter->convert(
                            "RUB",
                            portfolioEntry.currency.data(),
                            portfolioEntry.sum);
                unknownCounter.tickers.push_back(portfolioEntry.ticker);
            }
        }
        gcList.list.push_back(unknownCounter);
        statistics.list.push_back(gcList);
    }
    {
        const auto &portfolioSum = entities.getPortfolioSum();
        StatisticsCounter gcList("Currency");
        for(const auto &currency : portfolioSum.list)
        {
            StatisticsGroupCounter groupCounter(currency.currency.data());
            gcList.list.push_back(groupCounter);
        }
        for(const auto &portfolioEntry : entities.portfolio)
        {
            auto f = [&portfolioEntry](auto const &gc)
            {return gc.name == portfolioEntry.currency.data();};
            auto groupCounter =
                    std::find_if(gcList.list.begin(), gcList.list.end(), f);
            assert(groupCounter != gcList.list.end());
            groupCounter->sum+= converter->convert(
                        "RUB",
                        portfolioEntry.currency.data(),
                        portfolioEntry.sum);
            groupCounter->tickers.push_back(portfolioEntry.ticker);
        }
        statistics.list.push_back(gcList);
    }
    {
        StatisticsCounter gcList("Trend");
        StatisticsGroupCounter positiveGroup("Positive");
        StatisticsGroupCounter negativeGroup("Negative");

        for(size_t i = 0; i < entities.portfolio.size(); ++i)
        {
            auto stock = entities.getStockForPortfolioEntry(i);
            auto portfolioEntry = entities.portfolio[i];
            if(stock.derivation > 0)
            {
                positiveGroup.sum+= converter->convert(
                            "RUB",
                            portfolioEntry.currency.data(),
                            portfolioEntry.sum);
                positiveGroup.tickers.push_back(portfolioEntry.ticker);
            }else
            {
                negativeGroup.sum+= converter->convert(
                            "RUB",
                            portfolioEntry.currency.data(),
                            portfolioEntry.sum);
                negativeGroup.tickers.push_back(portfolioEntry.ticker);
            }
        }

        gcList.list.push_back(positiveGroup);
        gcList.list.push_back(negativeGroup);
        statistics.list.push_back(gcList);
    }
    {
        StatisticsCounter gcList("Plugins");

        {
            std::set<QString> plugins;
            std::transform(entities.portfolio.begin(),
                           entities.portfolio.end(),
                           std::inserter(plugins, plugins.begin()),
                           [](const auto &entry){return entry.plugin;});
            for(const auto &plugin : plugins)
            {
                StatisticsGroupCounter groupCounter(plugin);
                gcList.list.push_back(groupCounter);
            }
        }
        for(const auto &portfolioEntry : entities.portfolio)
        {
            auto f = [&portfolioEntry](auto const &gc)
            {return gc.name == portfolioEntry.plugin;};
            auto groupCounter =
                    std::find_if(gcList.list.begin(), gcList.list.end(), f);
            assert(groupCounter != gcList.list.end());
            groupCounter->sum+= converter->convert(
                        "RUB",
                        portfolioEntry.currency.data(),
                        portfolioEntry.sum);
            groupCounter->tickers.push_back(portfolioEntry.ticker);
        }
        statistics.list.push_back(gcList);
    }
    for(auto &entry : statistics.list)
    {
        for(auto &groupCounter : entry.list)
        {
            groupCounter.fractionPercent = groupCounter.sum
                    / statistics.totalSum
                    * 100;
        }
    }
    return statistics;
}
