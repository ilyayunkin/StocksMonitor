#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <Application/Application.h>

namespace Ui {
class StatisticsWidget;
}

class StatisticsWidget : public QWidget
{
    Q_OBJECT

    Application &application;
    void updateStocksList(const QString &pluginSelected);
public:
    explicit StatisticsWidget(Application &application,
                              QWidget *parent = nullptr);
    ~StatisticsWidget();

private:
    Ui::StatisticsWidget *ui;
};

#endif // STATISTICSWIDGET_H
