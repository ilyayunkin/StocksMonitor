#include "mainwindow.h"

#include <QApplication>

#include "stocksmonitor.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Logger::instance().log(__PRETTY_FUNCTION__);

    MainWindow w;
    w.showMaximized();
    StocksMonitor monitor(w.getModel());
    QObject::connect(&monitor, &StocksMonitor::downloaded,
                     &w, &MainWindow::lastTime);

    return a.exec();
}
