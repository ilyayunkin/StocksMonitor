#ifndef ABSTRACTSTATISTICSCONFIGDATABASE_H
#define ABSTRACTSTATISTICSCONFIGDATABASE_H

#include "Entities/Statistics.h"
#include "Entities/StockId.h"

class AbstractStatisticsConfigDatabase
{
public:
    virtual ~AbstractStatisticsConfigDatabase() = default;
    virtual StatisticsConfigList getAll() = 0;

    virtual QStringList getAllCategories() = 0;
    virtual QStringList getAllGroups(const QString &category) = 0;
    virtual StockIdList getAllItems(const QString &group) = 0;

    virtual void addCategory(const QString &category) = 0;
    virtual void addGroup(const QString &category, const QString &group) = 0;
    virtual void addItem(const QString &group, const StockId &item) = 0;

    virtual void removeCategory(const QString &category) = 0;
    virtual void removeGroup(const QString &category, const QString &group) = 0;
    virtual void removeItem(const QString &group, const char *const ticker) = 0;
};

#endif // ABSTRACTSTATISTICSCONFIGDATABASE_H
