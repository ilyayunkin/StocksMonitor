#include "Application.h"

#include "logger.h"
#include "ExceptionClasses.h"
#include "SourcePluginInterface.h"
#include "CurrencyConverter.h"
#include "Rules/RulesFasade.h"
#include "Rules/StocksMonitor.h"
#include "BuyRequestDatabase.h"
#include "PortfolioDatabase.h"

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

    rules = new RulesFasade();
    StocksInterface *currencyStocksInterface = nullptr;
    for(PluginsList::size_type i = 0; i < plugins.size(); ++i)
    {
        auto plugin = plugins.at(i);
        auto db = std::make_shared<BuyRequestDatabase>(plugin->getName());
        buyRequestDatabases.push_back(db);
        auto handler = rules->addStocksSource(
                    StocksSource{plugin->getName(),
                                 plugin->getCurrencyCode(),
                                 db.get()});

        auto monitor = std::make_shared<StocksMonitor>(*rules,
                                                       handler,
                                                       plugin->createParser(),
                                                       plugin->getUrl());
        monitors.push_back(monitor);
    }
    for(auto &interface : rules->getViewInterfaces())
    {
        if(interface.name == "CBRF-Currency")
        {
            currencyStocksInterface = &interface.stocks;
        }
    }
    converter = new CurrencyConverter("RUB", currencyStocksInterface);
    rules->setConverter(converter);
    portfolioDatabase = std::make_shared<PortfolioDatabase>();
    rules->setPortfolioDatabase(portfolioDatabase.get());
}

Application::~Application()
{
    delete rules;
    delete converter;
}

ViewInterfaces &Application::getViewInterfaces()
{
    return rules->getViewInterfaces();
}

QString Application::getPortfolioPrice(const char * const currency)
{
    return rules->getPortfolioPrice(currency);
}

QString Application::getPortfolioPrice()
{
    return rules->getPortfolioPrice();
}

QStringList Application::getAvailibleCurrencies()
{
    return rules->getAvailibleCurrencies();
}

PortfolioInterface &Application::getPortfolioInterface()
{
    return rules->getPortfolioInterface();
}

void Application::setNotifier(AbstractNotifier * const notifier)
{
    rules->setNotifier(notifier);
}

void Application::setDialogs(AbstractDialogs * const dialogs)
{
    rules->setDialogs(dialogs);
}
