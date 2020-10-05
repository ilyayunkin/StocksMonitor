#ifndef StocksLimitsModel_H
#define StocksLimitsModel_H

#include <QAbstractTableModel>

#include "Rules/StockLimit.h"
#include "Application/StocksLimitsDatabase.h"
#include "Rules/AbstractStocksView.h"
#include "Color.h"

class AbstractStocksModel;

class StocksLimitsModel final: public QAbstractTableModel, public AbstractStocksView
{
    Q_OBJECT

    StocksLimitsDatabase &stockLimits;
    mutable size_t size;

public:
    enum
    {
        NAME,
        TICKER,
        PRICE,
        BASE_PRICE,
        DISTANCE,

        COL_COUNT
    };
    explicit StocksLimitsModel(StocksLimitsDatabase &stockLimits,
                               QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void stocksUpdated() override;
    void stocksUpdated(size_t row) override;
};

#endif // StocksLimitsModel_H
