#include "StatisticsController.h"
#include "Rules/StatisticsInteractor.h"
#include "Application/AbstractStatisticsView.h"
#include "ExceptionClasses.h"
#include "AbstractStatisticsConfigView.h"

#include <QMessageBox>

StatisticsController::StatisticsController(StatisticsInteractor &rules,
                                           AbstractStatisticsView &view)
    : rules(rules)
    , resultView(view)
{
}

void StatisticsController::setConfigView(AbstractStatisticsConfigView *configView)
{
    this->configView = configView;
}

const StatisticsConfigList &StatisticsController::getStatisticsConfig() const
{
    return rules.getStatisticsConfig();
}

bool StatisticsController::addItem(const StatisticsConfigItemIndex &index)
{
    try {
        bool ok = rules.addStatisticsItem(index.category, index.group, index.stock);
        configView->addItem(index);
        return ok;
    } catch (ItemAlreadyIsInStatisticsConfig &e) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Error"),
                              QObject::tr("Item is already added to this category"));
    }
    return false;
}

bool StatisticsController::addGroup(const StatisticsConfigGroupIndex &index)
{
    bool ok = rules.addStatisticsGroup(index.category
                                       , index.group);
    if(ok)
    {
        configView->addGroup(index);
    }
    return ok;
}

bool StatisticsController::addCategory(const StatisticsConfigCategoryIndex &index)
{
    bool ok = rules.addStatisticsCategory(index.category);
    if(ok)
    {
        configView->addCategory(index);
    }
    return ok;
}

bool StatisticsController::removeItem(const StatisticsConfigItemIndex &index)
{
    bool ok = rules.removeStatisticsItem(index.category
                                         , index.group
                                         , index.stock.ticker.data());
    if(ok)
    {
        configView->removeItem(index);
    }
    return ok;
}

bool StatisticsController::removeGroup(const StatisticsConfigGroupIndex &index)
{
    bool ok = rules.removeStatisticsGroup(index.category, index.group);
    if(ok)
    {
        configView->removeGroup(index);
    }
    return ok;
}

bool StatisticsController::removeCategory(const StatisticsConfigCategoryIndex &index)
{
    bool ok = rules.removeStatisticsCategory(index.category);
    if(ok)
    {
        configView->removeCategory(index);
    }
    return ok;
}

void StatisticsController::processStatistics() const
{
    resultView.show(rules.processStatistics());
}
