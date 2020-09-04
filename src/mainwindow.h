#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSound>

#include "ModelsReference.h"
#include "PocketModel.h"

#include <memory>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    std::shared_ptr<QSound> sound;

    PocketModel &pocketModel;
    ModelsReferenceList &models;

    void selectSoundFile();
    void setupFile(const QString &filename);
    void signalize();
    void crossedLimit(const StockLimit &stockLimit);
    void closeEvent(QCloseEvent *event);
public:
    MainWindow(PocketModel &pocketModel, ModelsReferenceList &models, QWidget *parent = nullptr);
    ~MainWindow();
    void save();
};
#endif // MAINWINDOW_H
