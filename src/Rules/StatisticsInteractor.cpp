#include "StatisticsInteractor.h"

#include "AbstractStatisticsConfigDatabase.h"
#include "AbstractDialogs.h"
#include "AbstractCurrencyConverter.h"
#include "Entities/Entities.h"
#include "StatisticsResults.h"

#include "ExceptionClasses.h"

#include <assert.h>
#include <set>

StatisticsInteractor::StatisticsInteractor(AbstractStatisticsConfigDatabase &db,
                                           Entities &entities)
    : db(db)
    , entities(entities)
{
    entities.statistics = db.getAll();
}

void StatisticsInteractor::setConverter(AbstractCurrencyConverter * const converter)
{
    this->converter = converter;
}

void StatisticsInteractor::setDialogs(AbstractDialogs * const dialogs)
{
    this->dialogs = dialogs;
}

const StatisticsConfigList &StatisticsInteractor::getStatisticsConfig() const
{
    return entities.statistics;
}

bool StatisticsInteractor::addStatisticsItem(const QString &category,
                                             const QString &group,
                                             const StockId &stock)
{
    const auto catIt = std::find_if(entities.statistics.begin()
                                    , entities.statistics.end()
                                    , [category](auto const &cat){return cat.name == category;});
    if(catIt == entities.statistics.end())
    {
        throw NoSuchCategoryInStatisticsConfig();
    }

    if(isItemInCategory(catIt, stock.ticker.data()))
    {
        throw ItemAlreadyIsInStatisticsConfig();
    }

    const auto groupIt = std::find_if(catIt->list.begin()
                                      , catIt->list.end()
                                      , [group](auto const &g){return g.name == group;});
    if(groupIt == catIt->list.end())
    {
        throw NoSuchGroupInStatisticsConfig();
    }

    groupIt->list.push_back(stock);
    db.addItem(groupIt->name, stock);

    return true;
}

bool StatisticsInteractor::addStatisticsGroup(const QString &category, const QString &groupName)
{
    const auto catIt = std::find_if(entities.statistics.begin()
                                    , entities.statistics.end()
                                    , [category](auto const &cat){return cat.name == category;});
    if(catIt == entities.statistics.end())
    {
        throw NoSuchCategoryInStatisticsConfig();
    }

    catIt->list.push_back(StatisticsGroupConfig(groupName));
    db.addGroup(catIt->name, groupName);
    return true;
}

bool StatisticsInteractor::addStatisticsCategory(const QString &entryName)
{
    entities.statistics.push_back(StatisticsCathegoryConfig(entryName));
    db.addCategory(entryName);
    return true;
}

bool StatisticsInteractor::removeStatisticsItem(const QString &category, const QString &group, const char *const ticker)
{
    assert(ticker);
    assert(strlen(ticker) != 0);
    const auto catIt = std::find_if(entities.statistics.begin()
                                    , entities.statistics.end()
                                    , [category](auto const &cat){return cat.name == category;});
    if(catIt == entities.statistics.end())
    {
        throw NoSuchCategoryInStatisticsConfig();
    }

    const auto groupIt = std::find_if(catIt->list.begin()
                                      , catIt->list.end()
                                      , [group](auto const &g){return g.name == group;});

    if(groupIt == catIt->list.end())
    {
        throw NoSuchGroupInStatisticsConfig();
    }

    const auto itemIt = std::find_if(groupIt->list.begin()
                                     , groupIt->list.end()
                                     , [ticker](auto const &i){return i.ticker == ticker;});
    if(itemIt == groupIt->list.end())
    {
        throw NoSuchItemInStatisticsConfig();
    }

    if(dialogs->askReplaceItemFromStatistics(ticker))
    {
        db.removeItem(group, ticker);
        groupIt->list.erase(itemIt);
        return true;
    }

    return false;
}

bool StatisticsInteractor::removeStatisticsGroup(const QString &category, const QString &group)
{
    const auto catIt = std::find_if(entities.statistics.begin()
                                    , entities.statistics.end()
                                    , [category](auto const &cat){return cat.name == category;});
    if(catIt == entities.statistics.end())
    {
        throw NoSuchCategoryInStatisticsConfig();
    }

    const auto groupIt = std::find_if(catIt->list.begin()
                                      , catIt->list.end()
                                      , [group](auto const &g){return g.name == group;});

    if(groupIt == catIt->list.end())
    {
        throw NoSuchGroupInStatisticsConfig();
    }

    if(dialogs->askReplaceGroupStatistics(group))
    {
        db.removeGroup(category, group);
        catIt->list.erase(groupIt);
        return true;
    }

    return false;
}

bool StatisticsInteractor::removeStatisticsCategory(const QString &category)
{
    const auto catIt = std::find_if(entities.statistics.begin()
                                    , entities.statistics.end()
                                    , [category](auto const &cat){return cat.name == category;});
    if(catIt == entities.statistics.end())
    {
        throw NoSuchCategoryInStatisticsConfig();
    }

    if(dialogs->askReplaceGroupStatistics(category))
    {
        db.removeCategory(category);
        entities.statistics.erase(catIt);
        return true;
    }
    return false;
}

Statistics StatisticsInteractor::processStatistics() const
{
    Statistics statistics;
    {
        auto const sum = converter->convert("RUB", getPortfolioSum());
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
        const auto &portfolioSum = getPortfolioSum();
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
            auto stock = getStockForPortfolioEntry(i);
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

CurrencyCountersList StatisticsInteractor::getPortfolioSum() const
{
    CurrencyCountersList counters;
    for(auto &e : entities.portfolio)
    {
        counters.add(e.currency.data(), e.price * e.quantity);
    }

    return counters;
}

Stock StatisticsInteractor::getStockForPortfolioEntry(const size_t i) const
{
    auto const &entry = entities.portfolio.at(i);
    return getStock(entry.handler, entry.ticker.data());
}

Stock StatisticsInteractor::getStock(const stocksListHandler handler, const char * const ticker) const
{
    assert(ticker);
    assert(strlen(ticker) != 0);
    const auto &stocks = entities.pairs[handler].stocks;

    Stock ret{};
    const auto cit = std::find_if(stocks.begin(), stocks.end(),
                                  [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        ret = *cit;
    }
    return ret;
}

bool StatisticsInteractor::isItemInCategory(const StatisticsConfigList::iterator category,
                                            const char *ticker)
{
    for(const auto &group : category->list)
    {
        for(const auto &item : group.list)
        {
            if(item.ticker == ticker)
            {
                return true;
            }
        }
    }
    return false;
}
