#include "Application.h"

#include "WidgetsUi/Sounds/Signalizer.h"
#include "WidgetsUi/PopUpWindow.h"

#include "logger.h"
#include "ExceptionClasses.h"
#include "SourcePluginInterface.h"
#include "CurrencyConverter.h"
#include "Rules/RulesFasade.h"
#include "StocksDatabase.h"
#include "WidgetsUi/PortfolioModel.h"
#include "Application/StocksLimitsDatabase.h"

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

    signalizer = new Signalizer;

    AbstractStocksModel *currencyModel = nullptr;
    for(PluginsList::size_type i = 0; i < plugins.size(); ++i)
    {
        auto plugin = plugins.at(i);
        auto name = plugin->getName();
        ModelsReference ref{name,
                    QByteArray(),
                    std::shared_ptr<AbstractStocksModel>(new StocksDatabase(name, plugin->getCurrencyCode())),
                    std::make_shared<StocksLimitsDatabase>(name)};
        ref.limitsModel->setStocksModel(ref.stocksModel.get());
        if(name == "CBRF-Currency")
        {
            currencyModel = ref.stocksModel.get();
        }
        QObject::connect(ref.limitsModel.get(), &StocksLimitsDatabase::boundCrossed,
                         [this]{signalizer->signalize();});
        QObject::connect(ref.limitsModel.get(), &StocksLimitsDatabase::crossedLimit,
                         [](const StockLimit &stockLimit)
        {
            QString logMessage = QObject::tr("Stock cheapened:\n%1 %2")
                    .arg(stockLimit.name)
                    .arg(stockLimit.price);
            Logger::instance().log(logMessage);
            PopUpWindow::showPopUpWindow(logMessage);
        });
        models.push_back(ref);
    }

    portfolio = new PortfolioModel(models);
    {
        QObject::connect(portfolio, &PortfolioModel::boundCrossed,
                         [this]{signalizer->signalize();});
        QObject::connect(portfolio, &PortfolioModel::crossedLimit,
                         [](const PortfolioEntry &entry)
        {
            QString logMessage = QObject::tr("Portfolio sell price reached:\n"
                                             "%1 %2")
                    .arg(entry.name)
                    .arg(entry.price);
            Logger::instance().log(logMessage);
            PopUpWindow::showPopUpWindow(logMessage);
        });
    }
    converter = new CurrencyConverter(currencyModel);

    rules = new RulesFasade(*converter, *portfolio, plugins, models);
}

Application::~Application()
{
    delete rules;
    delete signalizer;
    delete portfolio;
    delete converter;
}

Application::PluginsList Application::loadPlugins()
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

ModelsReferenceList &Application::modelsReferences()
{
    return models;
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

Signalizer *Application::getSignalizer()
{
    return  signalizer;
}

PortfolioModel *Application::getPortfolio()
{
    return portfolio;
}
