#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLabel>

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
    QLabel *statusLabel;

    void stockDoubleClicked(const QModelIndex &index);
    void signalize();
    void crossedLimit(const StockLimit &stockLimit);
    void closeEvent(QCloseEvent *event);
public:
    MainWindow(QWidget *parent = nullptr);
    AbstractStocksModel &getModel(){return model;}
    ~MainWindow();
    void save();
    void lastTime(const QByteArray &time);
};
#endif // MAINWINDOW_H
