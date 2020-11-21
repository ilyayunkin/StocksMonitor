#include "StocksLimitsModel.h"

#include <QBrush>
#include <QMessageBox>
#include <QDebug>
#include <QMimeData>
#include <QInputDialog>

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
            Qt::ItemIsDropEnabled |
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
    if ((role == Qt::DisplayRole) && (orientation == Qt::Horizontal)){
        if((section >= NAME) && (section < COL_COUNT)){
            static const QString headers[] ={
                tr("Name")
                , tr("TICKER")
                , tr("Price")
                , tr("Base Price")
                , tr("Distance")
            };
            return headers[section];
        }
    }
    return QVariant();
}
QVariant StocksLimitsModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    assert(row >= 0);
    assert(col >= 0);

    const auto uRow = static_cast<size_t>(row);
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole)){
        if(uRow < (size = stockLimits.size())){
            const StockLimit &limit = stockLimits.getStockBuyRequest(row);
            switch (col) {
            case NAME: return limit.name;
            case TICKER: return limit.ticker.data();
            case PRICE: return limit.price;
            case BASE_PRICE: return limit.basePrice;
            case DISTANCE: return (limit.price - limit.basePrice) / limit.basePrice;
            default: break;
            }
        }
    }else if (role == Qt::BackgroundRole){
        if(uRow < (size = stockLimits.size())){
            const StockLimit &limit = stockLimits.getStockBuyRequest(row);
            return LimitBackgrounColor::brushForColor(
                        LimitBackgrounColor::colorForDistance(
                            (limit.price - limit.basePrice) / limit.basePrice));
        }
    }else if (role == Qt::ToolTipRole){
        if(uRow < (size = stockLimits.size())){
            const StockLimit &limit = stockLimits.getStockBuyRequest(row);
            return StockHint::getHint(stocksInterface.getStock(limit.ticker.data()));
        }
    }
    return QVariant();
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

QString StocksLimitsModel::mimeType() const
{
    return "application/x-ticker";
}

QStringList StocksLimitsModel::mimeTypes() const
{
    return QStringList() << mimeType();
}

bool StocksLimitsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column);
    Q_UNUSED(action)
    Q_UNUSED(row);
    Q_UNUSED(parent);
    bool ret = true;

    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << data->data(mimeType());
    auto ticker = data->data(mimeType());
    if(!ticker.isEmpty())
    {
        stocksInterface.addLimit(ticker.data());
    }

    return ret;
}


Qt::DropActions StocksLimitsModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
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
