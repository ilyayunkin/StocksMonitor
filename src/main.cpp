#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

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

        QDir pluginsDir( "./plugins" );
        SourcePluginInterface* plugin = nullptr;

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
