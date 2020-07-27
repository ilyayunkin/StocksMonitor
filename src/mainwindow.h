#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QLabel>
#include <QSound>

#include "StocksModel.h"
#include "StocksLimitsModel.h"

#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    std::shared_ptr<QSound> sound;

    StocksModel model;
    StocksLimitsModel limitsModel;
    QTableView *stocksTableView;
    QTableView *stocksLimitsTableView;
    QLabel *statusLabel;

    void stockDoubleClicked(const QModelIndex &index);
    void selectSoundFile();
    void setupFile(const QString &filename);
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
