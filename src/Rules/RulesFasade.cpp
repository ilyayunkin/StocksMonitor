#include "RulesFasade.h"

#include <algorithm>
#include <set>
#include <assert.h>

#include <QDebug>

#include "AbstractCurrencyConverter.h"
#include "AbstractBuyRequestDatabase.h"
#include "AbstractPortfolioDatabase.h"
#include "AbstractStatisticsConfigDatabase.h"

QStringList RulesFasade::getPluginsList() const
{
    QStringList ret;
    std::transform(entities.pairs.begin(), entities.pairs.end(),
                   std::back_inserter(ret), [](const auto &pair){return pair.name;});
    return ret;
}

StockIdList RulesFasade::getStockIdList(const PluginName &plugin) const
{
    const auto &it = std::find_if(entities.pairs.begin(), entities.pairs.end(),
                                  [&plugin](const auto &pair){return plugin == pair.name;});
    if(it != entities.pairs.end())
    {
        StockIdList ret;
        std::transform(it->stocks.begin(), it->stocks.end(),
                       std::back_inserter(ret),
                       [](const auto &stock){return StockId(stock.ticker, stock.name);});
        return ret;
    }
    return StockIdList();
}

RulesFasade::RulesFasade(AbstractStatisticsConfigDatabase *statisticsDb,
                         AbstractCurrencyConverter &converter,
                         const AbstractDialogs &dialogs)
    : entities()
    , subscriptions()
    , statisticsDb(statisticsDb)
    , statisticsInteractor(*statisticsDb, entities, dialogs)
    , processStatisticsInteractor(entities.portfolio, entities.statistics, converter)
    , loadStocksInteractor(entities, subscriptions)
    , editPortfolioInteractor(entities, subscriptions, converter, dialogs)
    , editBuyRequestInteractor(entities, subscriptions, dialogs)
    , converter(converter)
    , dialogs(dialogs)
{
    qDebug() << __PRETTY_FUNCTION__;
}

stocksListHandler RulesFasade::addStocksSource(const StocksSource &source)
{
    stocksListHandler handler = entities.pairs.size();

    entities.pairs.push_back(StockListsPair{source.name,
                                            source.currencyCode,
                                            source.db->getAll()});
    editBuyRequestInteractor.addDatabase(source.db);
    entities.portfolio.registerStockSourceInPortfolio(source.name, handler);
    return handler;
}

RulesFasade::~RulesFasade()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QStringList RulesFasade::getAvailibleCurrencies()
{
    auto counters = entities.portfolio.sum();

    assert(!counters.list.empty());

    QStringList items;
    std::transform(counters.list.begin(), counters.list.end(),
                   std::back_inserter(items),
                   [](const CurrencyCounter &c){return c.currency.data();});

    return items;
}

