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
public:
    explicit StatisticsWidget(Application &application,
                              QWidget *parent = nullptr);
    ~StatisticsWidget();

    // AbstractStatisticsConfigView interface
    void addItem(const StatisticsConfigItemIndex &index) override;
    void addGroup(const StatisticsConfigGroupIndex &index) override;
    void addCategory(const StatisticsConfigCategoryIndex &index) override;
    void removeItem(const StatisticsConfigItemIndex &index) override;
    void removeGroup(const StatisticsConfigGroupIndex &index) override;
    void removeCategory(const StatisticsConfigCategoryIndex &index) override;

private:
    Ui::StatisticsWidget *ui;

    Application &application;
    StatisticsController &configController;
    StatisticsModel *model = nullptr;

    void updateStocksList(const QString &pluginSelected);
    void updateEnables(const QModelIndex &index);
    void addStatisticsConfigElement();
    void removeSelectedConfigTreeElement();
};

#endif // STATISTICSWIDGET_H
