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

    Application &application;
    Signalizer *signalizer;
    Notifier *notifier;

    void selectSoundFile();
    void setupFile(const QString &filename);
    void closeEvent(QCloseEvent *event);
public:
    MainWindow(Application &application,
               QWidget *parent = nullptr);
    ~MainWindow();
    void save();
};
#endif // MAINWINDOW_H
