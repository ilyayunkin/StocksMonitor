#ifndef STATISTICSCONFIGDATABASE_H
#define STATISTICSCONFIGDATABASE_H

#include <QtSql>

#include "Rules/AbstractStatisticsConfigDatabase.h"

class StatisticsConfigDatabase final : public AbstractStatisticsConfigDatabase
{
public:
    StatisticsConfigDatabase();
    ~StatisticsConfigDatabase();
    StatisticsConfigList getAll()const override;
    bool isEmpty()const override;
    size_t getCategoryCount()const override;
    size_t getGroupCount(const QString &category)const override;
    size_t getItemsCount(const QString &category,
                         const QString &group)const override;

    QStringList getAllCategories()const override;
    QStringList getAllGroups(const QString &category)const override;
    StockIdList getAllItems(const QString &group)const override;

    void addCategory(const QString &category) override;
    void addGroup(const QString &category, const QString &group) override;
    void addItem(const QString &group, const StockId &item) override;

    void removeCategory(const QString &category) override;
    void removeGroup(const QString &category, const QString &group) override;
    void removeItem(const QString &group, const char *const ticker) override;
private:
    QSqlDatabase db;
    QSqlQuery executeQuery(const QString &query)const;
    QSqlQuery executeQueryException(const QString &query)const;
};

#endif // STATISTICSCONFIGDATABASE_H
