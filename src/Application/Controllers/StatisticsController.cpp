#include "StatisticsController.h"
#include "Rules/StatisticsInteractor.h"
#include "Application/AbstractStatisticsView.h"
#include "ExceptionClasses.h"

#include <QMessageBox>

StatisticsController::StatisticsController(StatisticsInteractor &rules,
                                           AbstractStatisticsView &view)
    : rules(rules)
    , view(view)
{
}

const StatisticsConfigList &StatisticsController::getStatisticsConfig() const
{
    return rules.getStatisticsConfig();
}

bool StatisticsController::addItem(const QString &entry,
                                  const QString &group,
                                  const StockId &stock)
{
    try {
        return rules.addStatisticsItem(entry, group, stock);
    } catch (ItemAlreadyIsInStatisticsConfig &e) {
        QMessageBox::critical(nullptr,
                              QObject::tr("Error"),
                              QObject::tr("Item is already added to this category"));
    }
    return false;
}

bool StatisticsController::addGroup(const QString &entry,
                                   const QString &groupName)
{
    return rules.addStatisticsGroup(entry, groupName);
}

bool StatisticsController::addCategory(const QString &entryName)
{
    return rules.addStatisticsCategory(entryName);
}

bool StatisticsController::removeItem(const QString &entry,
                                     const QString &group,
                                     const char * const ticker)
{
    return rules.removeStatisticsItem(entry, group, ticker);
}

bool StatisticsController::removeGroup(const QString &entry,
                                      const QString &group)
{
    return rules.removeStatisticsGroup(entry, group);
}

bool StatisticsController::removeCategory(const QString &entry)
{
    return rules.removeStatisticsCategory(entry);
}

void StatisticsController::processStatistics() const
{
    view.show(rules.processStatistics());
}
