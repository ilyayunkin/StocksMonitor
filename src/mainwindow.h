#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ModelsReference.h"
#include "Sounds/AbstractSignalizer.h"

class AbstractCurrencyConverter;
class PocketModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    PocketModel &pocketModel;
    ModelsReferenceList &models;

    void selectSoundFile();
    void setupFile(const QString &filename);
    void closeEvent(QCloseEvent *event);
public:
    MainWindow(PocketModel &pocketModel,
               ModelsReferenceList &models,
               AbstractSignalizer &signalizer,
               AbstractCurrencyConverter &converter,
               QWidget *parent = nullptr);
    ~MainWindow();
    void save();
};
#endif // MAINWINDOW_H
