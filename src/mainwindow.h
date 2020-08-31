#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QSound>

#include "ModelsReference.h"

#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    std::shared_ptr<QSound> sound;

    ModelsReferenceList &models;
    QLabel *statusLabel;

    void selectSoundFile();
    void setupFile(const QString &filename);
    void signalize();
    void crossedLimit(const StockLimit &stockLimit);
    void closeEvent(QCloseEvent *event);
public:
    MainWindow(ModelsReferenceList &models, QWidget *parent = nullptr);
//    AbstractStocksModel &getModel(){return model;}
    ~MainWindow();
    void save();
    void lastTime(const QByteArray &time);
};
#endif // MAINWINDOW_H
