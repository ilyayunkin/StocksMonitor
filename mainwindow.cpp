#include "mainwindow.h"
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *w = new QWidget;
    setCentralWidget(w);

    QHBoxLayout *hlay = new QHBoxLayout(w);
    tw = new QTableView;
    hlay->addWidget(tw);
    tw->setModel(&model);
}

MainWindow::~MainWindow()
{
}

