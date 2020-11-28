#include "Application.h"

#include "logger.h"
#include "ExceptionClasses.h"
#include "SourcePluginInterface.h"
#include "CurrencyConverter.h"
#include "Rules/RulesFasade.h"
#include "StocksMonitor.h"
#include "BuyRequestDatabase.h"
#include "PortfolioDatabase.h"
#include "StatisticsCsvSaver.h"
#include "StatisticsConfigDatabase.h"
#include "Controllers/StatisticsController.h"
#include "Controllers/ProcessStatisticsController.h"

namespace  {
typedef std::vector<std::unique_ptr<SourcePluginInterface>> PluginsList;

PluginsList loadPlugins()
{
    QDir pluginsDir("./plugins");
    PluginsList plugins;

    if(pluginsDir.entryList( QDir::Files ).isEmpty())
    {
        Logger::instance().log("No files");
        throw NoPluginsException();
    }

    for(const QString& fileName : pluginsDir.entryList(QStringList("*.dll"), QDir::Files ))
    {
        Logger::instance().log(QObject::tr("File found: ")
                               + fileName);

        QPluginLoader loader( pluginsDir.absoluteFilePath( fileName ) );
        if(loader.load() )
        {
            if(SourcePluginInterface* pluginPtr =
                    qobject_cast< SourcePluginInterface* >( loader.instance() ))
            {
                Logger::instance().log(QObject::tr("Availible plugin: ")
                                       + pluginPtr->getName());
                plugins.emplace_back(pluginPtr);
            }else
            {
                Logger::instance().log(QObject::tr("Failed to cast : ")
                                       + loader.errorString());
                loader.unload();
            }
        } else
        {
            Logger::instance().log(QObject::tr("Failed to load : ")
                                   + loader.errorString());
        }
    }
    return plugins;
}
}

Application::Application(QObject *parent) :
    QObject(parent)
  , csvSaver(new StatisticsCsvSaver)
  , statisticsConfigDatabase(new StatisticsConfigDatabase)
  , rules(new RulesFasade(statisticsConfigDatabase.get()))
  , portfolioDatabase(new PortfolioDatabase)
  , statisticsController(new StatisticsController(rules->getStatisticsInteractor()))
  , processStatisticsController(new ProcessStatisticsController(
                                    rules->getProcessStatisticsInteractor(),
                                    *csvSaver))
{
    QDir pluginsDir( "./plugins" );
    PluginsList plugins = loadPlugins();

    if(plugins.empty())
    {
        Logger::instance().log("Plugins not found");
        throw NoPluginsException();
    }

    for(const auto &plugin : plugins)
    {
        buyRequestDatabases.emplace_back(new BuyRequestDatabase(plugin->getName()));
        auto handler = rules->addStocksSource(
                    StocksSource{plugin->getName(),
                                 plugin->getCurrencyCode(),
                                 buyRequestDatabases.back().get()});

        viewInterfaces.push_back(ViewInterfacesPair(plugin->getName(),
                                                    rules->getEntities(),
                                                    rules->getSubscriptions(),
                                                    rules->getLoadStocksInteractor(),
                                                    rules->getEditPortfolioInteractor(),
                                                    rules->getEditBuyRequestInteractor(),
                                                    handler));
        monitors.emplace_back(new StocksMonitor(
                                  rules->getLoadStocksInteractor(),
                                  handler,
                                  plugin->createParser(),
                                  plugin->getUrl()));
    }
    auto currencyStocksInterface = [&]{
        auto cbrfInterface = std::find_if(viewInterfaces.begin(), viewInterfaces.end(),
                                          [](const auto &interface){return interface.name == "CBRF-Currency";});
        if(cbrfInterface != viewInterfaces.end())
            return &(cbrfInterface->stocksInterfaces);
        return static_cast<StocksInterface *>(nullptr);
    }();
    converter.reset(new CurrencyConverter("RUB", currencyStocksInterface));
    rules->setConverter(converter.get());
    rules->getEditPortfolioInteractor().setPortfolioDatabase(portfolioDatabase.get());
    portfolioInterface.reset(new PortfolioInterface(
                                 rules->getEntities(),
                                 rules->getSubscriptions(),
                                 rules->getEditPortfolioInteractor()));
}

Application::~Application()
{
}

QString Application::getPortfolioPrice(const char * const currency)
{
    return rules->getEditPortfolioInteractor().getPortfolioPrice(currency);
}

QString Application::getPortfolioPrice()
{
    return rules->getEditPortfolioInteractor().getPortfolioPrice();
}

QStringList Application::getAvailibleCurrencies()
{
    return rules->getAvailibleCurrencies();
}

QStringList Application::getPluginsList() const
{
    return rules->getPluginsList();
}

StockIdList Application::getStockIdList(const QString &plugin) const
{
    return rules->getStockIdList(plugin);
}

StatisticsController &Application::getStatisticsController()
{
    return *statisticsController;
}

void Application::setNotifier(AbstractNotifier * const notifier)
{
    rules->getLoadStocksInteractor().setNotifier(notifier);
}

void Application::setDialogs(AbstractDialogs * const dialogs)
{
    rules->setDialogs(dialogs);
}

void Application::setStatisticsConfigView(AbstractStatisticsConfigView *configView)
{
    statisticsController->setConfigView(configView);
}
