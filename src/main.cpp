#include "WidgetsUi/mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

#include <vector>

#include "stocksmonitor.h"
#include "logger.h"
#include "SourcePluginInterface.h"
#include "ExceptionClasses.h"
#include "ModelsReference.h"
#include "StocksLimitsModel.h"
#include "StocksModel.h"
#include "PocketModel.h"
#include "WidgetsUi/Sounds/Signalizer.h"
#include "WidgetsUi/PopUpWindow.h"
#include "CurrencyConverter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Ilya");
    a.setApplicationName("StocksMonitor");

    int ret = -1;

    try
    {
        Logger::instance().log("The program is started");

        QDir pluginsDir( "./plugins" );
        typedef std::vector<std::shared_ptr<SourcePluginInterface>> PluginsList;
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

        if(plugins.empty())
        {
            Logger::instance().log("Plugins not found");
            throw NoPluginsException();
        }

        Signalizer signalizer;

        ModelsReferenceList models;
        StocksModel *currencyModel = nullptr;
        for(PluginsList::size_type i = 0; i < plugins.size(); ++i)
        {
            auto plugin = plugins.at(i);
            auto name = plugin->getName();
            ModelsReference ref{name,
                        QByteArray(),
                        std::make_shared<StocksModel>(name, plugin->getCurrencyCode()),
                        std::make_shared<StocksLimitsModel>(name)};
            ref.limitsModel->setStocksModel(ref.stocksModel.get());
            if(name == "CBRF-Currency")
            {
                currencyModel = ref.stocksModel.get();
            }
            QObject::connect(ref.limitsModel.get(), &StocksLimitsModel::boundCrossed,
                             [&signalizer]{signalizer.signalize();});
            QObject::connect(ref.limitsModel.get(), &StocksLimitsModel::crossedLimit,
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

        PocketModel pocketModel(models);
        {
            QObject::connect(&pocketModel, &PocketModel::boundCrossed,
                             [&signalizer]{signalizer.signalize();});
            QObject::connect(&pocketModel, &PocketModel::crossedLimit,
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
        CurrencyConverter converter(currencyModel);

        MainWindow w(pocketModel, models, signalizer, converter);
        for(auto &ref : models)
        {
            QObject::connect(ref.limitsModel.get(), &StocksLimitsModel::boundCrossed,
                             [&w](){QApplication::alert(&w);});
        }
        w.showMaximized();

        std::vector<std::shared_ptr<StocksMonitor>> monitors;
        for(PluginsList::size_type i = 0; i < plugins.size(); ++i)
        {
            auto plugin = plugins[i];
            auto modelsRef = models[i];
            auto monitor = std::make_shared<StocksMonitor>(*(modelsRef.stocksModel.get()),
                                                           plugin->createParser(),
                                                           plugin->getUrl());
            monitors.push_back(monitor);
            QObject::connect(monitor.get(), &StocksMonitor::downloaded,
                             [&models, i](const QByteArray t){models[i].time = t;});
        }

        ret = a.exec();
    }catch(std::runtime_error &e)
    {
        Logger::instance().log(e.what());
        QMessageBox::critical(0, QObject::tr("Critical error"),
                              QObject::tr("Application will be closed: %1"
                                          "\nCheck the log file.").arg(e.what()));
        std::terminate();
    }catch (...)
    {
        Logger::instance().log("Undefined exception");
        QMessageBox::critical(0, QObject::tr("Critical error"),
                              QObject::tr("Application will be closed: "
                                          "Undefined exception"
                                          "\nCheck the log file."));
        std::terminate();
    }

    return ret;
}
