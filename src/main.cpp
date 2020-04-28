#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QDebug>

#include "stocksmonitor.h"
#include "logger.h"
#include "SourcePluginInterface.h"
#include "ExceptionClasses.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int ret = -1;

    try
    {
        Logger::instance().log(__PRETTY_FUNCTION__);

        MainWindow w;
        w.showMaximized();

        QDir pluginsDir( "." );
        SourcePluginInterface* plugin = nullptr;

        if(pluginsDir.entryList( QDir::Files ).isEmpty())
        {
            Logger::instance().log("No files");
            throw NoPluginsException();
        }

        foreach( const QString& fileName, pluginsDir.entryList( QDir::Files ) )
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
                    plugin = pluginPtr;
                    break;
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

        if(plugin == nullptr)
        {
            Logger::instance().log("Plugins not found");
            throw NoPluginsException();
        }

        StocksMonitor monitor(w.getModel(),
                              plugin->createParser(),
                              plugin->getUrl());
        QObject::connect(&monitor, &StocksMonitor::downloaded,
                         &w, &MainWindow::lastTime);
        ret = a.exec();
    }catch(std::runtime_error &e)
    {
        Logger::instance().log(e.what());
        std::terminate();
    }catch (...)
    {
        Logger::instance().log("Undefined exception");
        std::terminate();
    }

    return ret;
}
