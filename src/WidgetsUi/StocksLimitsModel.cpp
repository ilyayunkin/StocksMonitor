#include "StocksLimitsModel.h"

#include <QBrush>
#include <QMessageBox>
#include <QDebug>

#include <algorithm>
#include <assert.h>

#include "Presenters/StockHint.h"
#include "Presenters/LimitBackgrounColor.h"
#include "Application/BuyRequestInterface.h"
#include "Application/StocksInterface.h"

StocksLimitsModel::StocksLimitsModel(BuyRequestInterface &stockLimits,
                                     StocksInterface &stocksInterface,
                                     QObject *parent) :
    QAbstractTableModel(parent),
    stockLimits(stockLimits),
    stocksInterface(stocksInterface)
{
    stockLimits.subscribeForChanges(this);
}

int StocksLimitsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return (size = stockLimits.size());
}

int StocksLimitsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COL_COUNT;
}

Qt::ItemFlags StocksLimitsModel::flags(const QModelIndex & index) const
{    
    Qt::ItemFlags ret =QAbstractTableModel::flags(index) |
            Qt::ItemIsEnabled |
            Qt::ItemIsSelectable;
    if(index.column() == BASE_PRICE)
    {
        ret |= Qt::ItemIsEditable;
    }

    return ret;
}

QVariant StocksLimitsModel::headerData(int section,
                                       Qt::Orientation orientation,
                                       int role) const
{
    QVariant ret;
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section) {
            case NAME:
                ret = tr("Name");
                break;
            case TICKER:
                ret = tr("TICKER");
                break;
            case PRICE:
                ret = tr("Price");
                break;
            case BASE_PRICE:
                ret = tr("Base Price");
                break;
            case DISTANCE:
                ret = tr("Distance");
                break;
            default:
                break;
            }
        }
    }
    return ret;
}
QVariant StocksLimitsModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    int row = index.row();
    int col = index.column();
    assert(row >= 0);
    assert(col >= 0);

    const auto uRow = static_cast<size_t>(row);
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if(uRow < (size = stockLimits.size()))
        {
            const StockLimit &limit = stockLimits.getStockBuyRequest(row);
            switch (col) {
            case NAME:
                ret = limit.name;
                break;
            case TICKER:
                ret = limit.ticker.data();
                break;
            case PRICE:
                ret = limit.price;
                break;
            case BASE_PRICE:
                ret = limit.basePrice;
                break;
            case DISTANCE:
                ret = (limit.price - limit.basePrice) / limit.basePrice;
                break;
            default:
                break;
            }
        }
    }
    if (role == Qt::BackgroundRole)
    {
        if(uRow < (size = stockLimits.size()))
        {
            const StockLimit &limit = stockLimits.getStockBuyRequest(row);
            ret = LimitBackgrounColor::brushForColor(
                        LimitBackgrounColor::colorForDistance(
                            (limit.price - limit.basePrice) / limit.basePrice));
        }
    }
    if (role == Qt::ToolTipRole)
    {
        if(uRow < (size = stockLimits.size()))
        {
            const StockLimit &limit = stockLimits.getStockBuyRequest(row);
            auto stock = stocksInterface.getStock(limit.ticker.data());
            ret = StockHint::getHint(stock);
        }
    }
    return ret;
}

bool StocksLimitsModel::setData(const QModelIndex &index,
                                const QVariant &value,
                                int role)
{
    int row = index.row();
    int col = index.column();
    assert(row >= 0);
    assert(col >= 0);

    const auto uRow = static_cast<size_t>(row);

    if ((role == Qt::EditRole)
            && (col == BASE_PRICE) && (uRow < (size = stockLimits.size())))
    {
        float referencePrice = value.toFloat();
        if(referencePrice > 0)
        {
            if(stockLimits.setReferencePrice(row, referencePrice))
            {
                emit dataChanged(createIndex(row, 0), createIndex(row, COL_COUNT - 1));
            }
            return true;
        }
    }
    return false;
}

void StocksLimitsModel::stocksUpdated()
{
    const auto newSize = stockLimits.size();
    if(size != 0 && (size == newSize))
    {
        emit dataChanged(createIndex(0, 0), createIndex((size = stockLimits.size()) - 1, COL_COUNT - 1));
    }else
    {
        if(size != 0)
        {
            beginRemoveRows(QModelIndex(), 0, size  - 1);
            endRemoveRows();
        }
        if((size = stockLimits.size()) != 0)
        {
            beginInsertRows(QModelIndex(), 0, (size = newSize) - 1);
            endInsertRows();
        }
    }
}

void StocksLimitsModel::stocksUpdated(size_t row)
{
    emit dataChanged(createIndex(row, 0), createIndex(row, COL_COUNT - 1));
}
