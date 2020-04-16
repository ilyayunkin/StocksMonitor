#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <StocksModel.h>
#include <StocksLimitsModel.h>
#include <QTableView>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    StocksModel model;
    StocksLimitsModel limitsModel;
    QTableView *stocksTableView;
    QTableView *stocksLimitsTableView;

    void stockDoubleClicked(const QModelIndex &index);
    void signalize();
public:
    MainWindow(QWidget *parent = nullptr);
    AbstractStocksModel &getModel(){return model;}
    ~MainWindow();
    void save();
};
#endif // MAINWINDOW_H
