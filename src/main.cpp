#include "mainwindow.h"

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

        foreach( const QString& fileName, pluginsDir.entryList(QStringList("*.dll"), QDir::Files ) )
        {
            qDebug() << "===============================================================================";
            qDebug() << "Found:" << fileName;

            QPluginLoader loader( pluginsDir.absoluteFilePath( fileName ) );
            if( loader.load() )
            {
                if( SourcePluginInterface* pluginPtr =
                        qobject_cast< SourcePluginInterface* >( loader.instance() ) )
                {
                    qDebug() << "Availible plugin: " << pluginPtr->getName();
                    plugins.emplace_back(pluginPtr);
                } else
                {
                    qDebug() << "Failed to cast :";
                    qDebug() << loader.errorString();
                    loader.unload();
                }
            } else
            {
                qDebug() << "Failed to load :(";
                qDebug() << loader.errorString();
            }

            qDebug() << "";
        }

        if(plugins.empty())
        {
            Logger::instance().log("Plugins not found");
            throw NoPluginsException();
        }

        ModelsReferenceList models;
        for(PluginsList::size_type i = 0; i < plugins.size(); ++i)
        {
            auto name = plugins[i]->getName();
            ModelsReference ref{name,
                        std::make_shared<StocksModel>(),
                        std::make_shared<StocksLimitsModel>(name)};
            ref.limitsModel->setStocksModel(ref.stocksModel.get());
            models.push_back(ref);
        }

        MainWindow w(models);
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
                             &w, &MainWindow::lastTime);
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
