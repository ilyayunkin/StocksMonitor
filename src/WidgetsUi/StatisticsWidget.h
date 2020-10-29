#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <Application/Application.h>

#include "Application/Controllers/AbstractStatisticsConfigView.h"

namespace Ui {
class StatisticsWidget;
}
class StatisticsModel;
class StatisticsWidget : public QWidget, public AbstractStatisticsConfigView
{
    Q_OBJECT

    Application &application;
    StatisticsController &configController;
    StatisticsModel *model = nullptr;
    void updateStocksList(const QString &pluginSelected);
public:
    explicit StatisticsWidget(Application &application,
                              QWidget *parent = nullptr);
    ~StatisticsWidget();

private:
    Ui::StatisticsWidget *ui;

    // AbstractStatisticsConfigView interface
public:
    void addItem(const StatisticsConfigItemIndex &index) override;
    void addGroup(const StatisticsConfigGroupIndex &index) override;
    void addCategory(const StatisticsConfigCategoryIndex &index) override;
    void removeItem(const StatisticsConfigItemIndex &index) override;
    void removeGroup(const StatisticsConfigGroupIndex &index) override;
    void removeCategory(const StatisticsConfigCategoryIndex &index) override;
};

#endif // STATISTICSWIDGET_H
