#ifndef STATISTICSCONFIGDATABASE_H
#define STATISTICSCONFIGDATABASE_H

#include <QtSql>

#include "Rules/AbstractStatisticsConfigDatabase.h"

class StatisticsConfigDatabase final : public AbstractStatisticsConfigDatabase
{
public:
    StatisticsConfigDatabase();
    ~StatisticsConfigDatabase();
    StatisticsConfigList getAll();

    QStringList getAllCategories();
    QStringList getAllGroups(const QString &category);
    StockIdList getAllItems(const QString &group);

    void addCategory(const QString &category);
    void addGroup(const QString &category, const QString &group);
    void addItem(const QString &group, const StockId &item);

    void removeCategory(const QString &category);
    void removeGroup(const QString &category, const QString &group);
    void removeItem(const QString &group, const char *const ticker);
private:
    QSqlDatabase db;
    QSqlQuery executeQuery(const QString &query);
    QSqlQuery executeQueryException(const QString &query);
};

#endif // STATISTICSCONFIGDATABASE_H
