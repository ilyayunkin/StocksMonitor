#include "StocksLimitsModel.h"

#include <QBrush>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>

#include <algorithm>
#include <assert.h>

#include "Presenters/StockHint.h"
#include "Presenters/LimitBackgrounColor.h"
#include "Rules/AbstractStocksModel.h"

StocksLimitsModel::StocksLimitsModel(StocksLimitsDatabase &stockLimits,
                                     QObject *parent) :
    QAbstractTableModel(parent),
    stockLimits(stockLimits)
{
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
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if(row < (size = stockLimits.size()))
        {
            const StockLimit &stock = stockLimits.getLimit(row);
            switch (col) {
            case NAME:
                ret = stock.name;
                break;
            case TICKER:
                ret = stock.ticker;
                break;
            case PRICE:
                ret = stock.price;
                break;
            case BASE_PRICE:
                ret = stock.basePrice;
                break;
            case DISTANCE:
                ret = (stock.price - stock.basePrice) / stock.basePrice;
                break;
            default:
                break;
            }
        }
    }
    if (role == Qt::BackgroundRole)
    {
        if(row < (size = stockLimits.size()))
        {
            ret = stockLimits.getBrush(row);
        }
    }
    if (role == Qt::ToolTipRole)
    {
        if(row < (size = stockLimits.size()))
        {
            ret = stockLimits.getToolTip(row);
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

    if ((role == Qt::EditRole) && (col == BASE_PRICE) && (row < (size = stockLimits.size())))
    {
        float v = value.toFloat();
        if(v > 0)
        {
            if(stockLimits.setReferencePrice(row, v))
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
    if(size != 0)
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
            beginInsertRows(QModelIndex(), 0, (size = stockLimits.size()) - 1);
            endInsertRows();
        }
    }
}

void StocksLimitsModel::stocksUpdated(size_t row)
{
    emit dataChanged(createIndex(row, 0), createIndex(row, COL_COUNT - 1));
}
