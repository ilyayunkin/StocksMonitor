#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <StocksModel.h>
#include <QTableView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    StocksModel model;
    QTableView *tw;

public:
    MainWindow(QWidget *parent = nullptr);
    AbstractStocksModel &getModel(){return model;}
    ~MainWindow();
};
#endif // MAINWINDOW_H
