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

namespace  {
typedef std::vector<std::shared_ptr<SourcePluginInterface>> PluginsList;

PluginsList loadPlugins()
{
    QDir pluginsDir("./plugins");
    PluginsList plugins;

    if(pluginsDir.entryList( QDir::Files ).isEmpty())
    {
        Logger::instance().log("No files");
        throw NoPluginsException();
    }

    foreach(const QString& fileName, pluginsDir.entryList(QStringList("*.dll"), QDir::Files ))
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
{
    QDir pluginsDir( "./plugins" );
    PluginsList plugins = loadPlugins();

    if(plugins.empty())
    {
        Logger::instance().log("Plugins not found");
        throw NoPluginsException();
    }

    StocksInterface *currencyStocksInterface = nullptr;
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
    for(auto &interface :  viewInterfaces)
    {
        if(interface.name == "CBRF-Currency")
        {
            currencyStocksInterface = &interface.stocksInterfaces;
        }
    }
    converter.reset(new CurrencyConverter("RUB", currencyStocksInterface));
    rules->setConverter(converter.get());
    portfolioDatabase.reset(new PortfolioDatabase);
    rules->getEditPortfolioInteractor().setPortfolioDatabase(portfolioDatabase.get());
    portfolioInterface.reset(new PortfolioInterface(
                                 rules->getEntities(),
                                 rules->getSubscriptions(),
                                 rules->getEditPortfolioInteractor()));
    statisticsController.reset(new StatisticsController(
                                   rules->getStatisticsInteractor(), *csvSaver));
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
