#ifndef StocksLimitsModel_H
#define StocksLimitsModel_H

#include <QAbstractTableModel>

#include <cstdlib>

#include "Rules/AbstractStocksView.h"
#include "Color.h"

class BuyRequestInterface;
class StocksInterface;

class StocksLimitsModel final: public QAbstractTableModel, public AbstractStocksView
{
    Q_OBJECT
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
    explicit StocksLimitsModel(BuyRequestInterface &stockLimits,
                               StocksInterface &stocksInterface,
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
private:
    BuyRequestInterface &stockLimits;
    StocksInterface &stocksInterface;
    mutable size_t size;
};

#endif // StocksLimitsModel_H
