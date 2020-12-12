#include "WidgetsUi/mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QMessageBox>

#include <vector>

#include "logger.h"
#include "ExceptionClasses.h"
#include "Application/Application.h"
#include "WidgetsUi/Dialogs.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Ilya");
    a.setApplicationName("StocksMonitor");

    int ret = -1;

    try
    {
        Logger::instance().log("The program is started");

        Dialogs dialogs;
        Application app(dialogs);

        MainWindow w(app);
        w.showMaximized();

        dialogs.setMainWindow(&w);

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
