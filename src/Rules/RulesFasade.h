#ifndef RULESFASADE_H
#define RULESFASADE_H

#include <cstdlib>
#include <memory.h>

#include "SourcePluginInterface.h"
#include "AbstractDialogs.h"
#include "AbstractStocksReceiver.h"
#include "Entities/Entities.h"
#include "Entities/StockId.h"
#include "Entities/Statistics.h"
#include "StatisticsInteractor.h"
#include "LoadStocksInteractor.h"
#include "EditPortfolioInteractor.h"
#include "EditBuyRequestInteractor.h"
#include "ProcessStatisticsInteractor.h"
#include "Subscriptions.h"

class AbstractBuyRequestDatabase;
struct StocksSource
{
    const QString name;
    const std::string currencyCode;
    AbstractBuyRequestDatabase *const db;
};

typedef std::vector<StocksSource> StocksSourceList;

class AbstractCurrencyConverter;
class AbstractPortfolioDatabase;
class AbstractNotifier;
class AbstractStatisticsConfigDatabase;

class RulesFasade
{
    Entities entities;
    Subscriptions subscriptions;
    AbstractStatisticsConfigDatabase *statisticsDb = nullptr;
    StatisticsInteractor statisticsInteractor;
    ProcessStatisticsInteractor processStatisticsInteractor;
    LoadStocksInteractor loadStocksInteractor;
    EditPortfolioInteractor editPortfolioInteractor;
    EditBuyRequestInteractor editBuyRequestInteractor;

    AbstractCurrencyConverter *converter = nullptr;
    AbstractDialogs *dialogs = nullptr;
public:
    explicit RulesFasade(AbstractStatisticsConfigDatabase *statisticsDb);
    ~RulesFasade();

    const Entities &getEntities(){return entities;}
    Subscriptions &getSubscriptions(){return subscriptions;}
    StatisticsInteractor &getStatisticsInteractor(){return statisticsInteractor;}
    LoadStocksInteractor &getLoadStocksInteractor(){return loadStocksInteractor;}
    EditPortfolioInteractor &getEditPortfolioInteractor(){return editPortfolioInteractor;}
    EditBuyRequestInteractor &getEditBuyRequestInteractor(){return editBuyRequestInteractor;}
    ProcessStatisticsInteractor &getProcessStatisticsInteractor(){return processStatisticsInteractor;}

    stocksListHandler addStocksSource(const StocksSource &source);
    void setConverter(AbstractCurrencyConverter *const converter);
    void setDialogs(AbstractDialogs *const dialogs);

    QStringList getAvailibleCurrencies();

    QStringList getPluginsList() const;
    StockIdList getStockIdList(const QString &plugin) const;
};

#endif // RULESFASADE_H
