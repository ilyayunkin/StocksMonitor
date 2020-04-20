#include "mainwindow.h"

#include <QApplication>
#include "stocksmonitor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    StocksMonitor monitor(w.getModel());
    QObject::connect(&monitor, &StocksMonitor::downloaded,
                     &w, &MainWindow::lastTime);

    return a.exec();
}
