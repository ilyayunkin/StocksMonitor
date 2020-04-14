#include "mainwindow.h"

#include <QApplication>
#include "stocksmonitor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    StocksMonitor monitor(w.getModel());

    return a.exec();
}
