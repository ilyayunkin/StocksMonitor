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
{
    QDir pluginsDir( "./plugins" );
    PluginsList plugins = loadPlugins();

    if(plugins.empty())
    {
        Logger::instance().log("Plugins not found");
        throw NoPluginsException();
    }

    csvSaver = std::make_shared<StatisticsCsvSaver>();
    statisticsConfigDatabase = std::make_shared<StatisticsConfigDatabase>();
    rules = std::make_shared<RulesFasade>(statisticsConfigDatabase.get());

    StocksInterface *currencyStocksInterface = nullptr;
    for(PluginsList::size_type i = 0; i < plugins.size(); ++i)
    {
        auto plugin = plugins.at(i);
        auto db = std::make_shared<BuyRequestDatabase>(plugin->getName());
        buyRequestDatabases.push_back(db);
        StocksSource source{plugin->getName(),
                     plugin->getCurrencyCode(),
                     db.get()};
        auto handler = rules->addStocksSource(source);

        viewInterfaces.push_back(ViewInterfacesPair(source.name,
                                                    rules->getEntities(),
                                                    rules->getSubscriptions(),
                                                    rules->getLoadStocksInteractor(),
                                                    rules->getEditPortfolioInteractor(),
                                                    rules->getEditBuyRequestInteractor(),
                                                    handler));
        auto monitor = std::make_shared<StocksMonitor>(rules->getLoadStocksInteractor(),
                                                       handler,
                                                       plugin->createParser(),
                                                       plugin->getUrl());
        monitors.push_back(monitor);
    }
    for(auto &interface :  viewInterfaces)
    {
        if(interface.name == "CBRF-Currency")
        {
            currencyStocksInterface = &interface.stocksInterfaces;
        }
    }
    converter = std::make_shared<CurrencyConverter>("RUB", currencyStocksInterface);
    rules->setConverter(converter.get());
    portfolioDatabase = std::make_shared<PortfolioDatabase>();
    rules->getEditPortfolioInteractor().setPortfolioDatabase(portfolioDatabase.get());
    portfolioInterface = std::make_shared<PortfolioInterface>(
                rules->getEntities(),
                rules->getSubscriptions(),
                rules->getEditPortfolioInteractor());
    statisticsController = std::make_shared<StatisticsController>(
                rules->getStatisticsInteractor(), *csvSaver);
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
