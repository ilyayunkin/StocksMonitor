#include "StatisticsInteractor.h"

#include "AbstractStatisticsConfigDatabase.h"
#include "AbstractDialogs.h"
#include "Entities/Entities.h"

#include "ExceptionClasses.h"

#include <assert.h>

StatisticsInteractor::StatisticsInteractor(AbstractStatisticsConfigDatabase &db,
                                           Entities &entities)
    : db(db)
    , entities(entities)
{
    entities.statistics = db.getAll();
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
