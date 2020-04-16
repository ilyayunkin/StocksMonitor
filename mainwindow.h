#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>

#include "StocksModel.h"
#include "StocksLimitsModel.h"

#include <QTextToSpeech>


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QTextToSpeech speaker;

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
