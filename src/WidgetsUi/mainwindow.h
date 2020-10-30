#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Application/Application.h"
#include "Sounds/AbstractSignalizer.h"
#include "Notifier.h"
#include "Sounds/Signalizer.h"

class AbstractCurrencyConverter;
class PortfolioModel;
class RulesFasade;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Application &application,
               QWidget *parent = nullptr);
    ~MainWindow();
    void save();
private:
    Application &application;
    Signalizer *signalizer;
    Notifier *notifier;

    void selectSoundFile();
    void setupFile(const QString &filename);
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
