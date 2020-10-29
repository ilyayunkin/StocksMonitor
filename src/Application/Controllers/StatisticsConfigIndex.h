#ifndef STATISTICSCONFIGINDEX_H
#define STATISTICSCONFIGINDEX_H

#include <QString>

#include "Entities/StockId.h"

struct StatisticsConfigItemIndex
{
    const QString category;
    const QString group;
    const StockId stock;
    void *internalPointer;
    StatisticsConfigItemIndex(const QString &category,
                              const QString &group,
                              const StockId &stock,
                              void *internalPointer)
        : category(category)
        , group(group)
        , stock(stock)
        , internalPointer(internalPointer)
    {}
};

struct StatisticsConfigGroupIndex
{
    const QString category;
    const QString group;
    void *internalPointer;
    StatisticsConfigGroupIndex(const QString &category,
                              const QString &group,
                              void *internalPointer)
        : category(category)
        , group(group)
        , internalPointer(internalPointer)
    {}
};

struct StatisticsConfigCategoryIndex
{
    const QString category;
    void *internalPointer;
    StatisticsConfigCategoryIndex(const QString &category,
                              void *internalPointer)
        : category(category)
        , internalPointer(internalPointer)
    {}
};

#endif // STATISTICSCONFIGINDEX_H
