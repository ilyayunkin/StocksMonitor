#include "StocksModel.h"

#include <QBrush>
#include <QDebug>

#include <algorithm>

StocksModel::StocksModel(StocksInterface &stocks, QObject *parent) :
    QAbstractTableModel(parent),
    stocks(stocks)
{
    qDebug() << __PRETTY_FUNCTION__ << __LINE__;
}

StocksModel::~StocksModel()
{
    qDebug() << __PRETTY_FUNCTION__ << __LINE__;
}

int StocksModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return stocks.size();
}

int StocksModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COL_COUNT;
}

Qt::ItemFlags StocksModel::flags(const QModelIndex & index) const
{    
    return QAbstractTableModel::flags(index) |
            Qt::ItemIsEnabled |
            Qt::ItemIsSelectable;
}

QVariant StocksModel::headerData(int section,
                                 Qt::Orientation orientation,
                                 int role) const
{
    QVariant ret;
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section) {
            case NUM:
                ret = tr("#");
                break;
            case NAME:
                ret = tr("Name");
                break;
            case TICKER:
                ret = tr("TICKER");
                break;
            case PRICE:
                ret = tr("Price");
                break;
            case DERIVATION:
                ret = tr("%\nDay");
                break;
            case DERIVATION_WEEK:
                ret = tr("%\nWeek");
                break;
            case DERIVATION_MONTH:
                ret = tr("%\nMonth");
                break;
            case DERIVATION_YEAR:
                ret = tr("%\nYear");
                break;
            default:
                break;
            }
        }
    }
    return ret;
}
QVariant StocksModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        int row = index.row();
        int col = index.column();
        if(row < (size = stocks.size()))
        {
            const Stock &stock = stocks.getStock(row);
            switch (col) {
            case NUM:
                ret = stock.rowNum;
                break;
            case NAME:
                ret = stock.name;
                break;
            case TICKER:
                ret = stock.ticker;
                break;
            case PRICE:
                ret = stock.price;
                break;
            case DERIVATION:
                ret = stock.derivation;
                break;
            case DERIVATION_WEEK:
                ret = stock.derivationWeek;
                break;
            case DERIVATION_MONTH:
                ret = stock.derivationMonth;
                break;
            case DERIVATION_YEAR:
                ret = stock.derivationYear;
                break;
            default:
                break;
            }
        }
    }
    if (role == Qt::ForegroundRole)
    {
        int row = index.row();
        int col = index.column();
        if(row < (size = stocks.size()))
        {
            const Stock &stock = stocks.getStock(row);
            float number = 0.0;
            switch (col) {
            case DERIVATION:
                number = stock.derivation;
                break;
            case DERIVATION_WEEK:
                number = stock.derivationWeek;
                break;
            case DERIVATION_MONTH:
                number = stock.derivationMonth;
                break;
            case DERIVATION_YEAR:
                number = stock.derivationYear;
                break;
            default:
                break;
            }
            if(number < 0.)
            {
                return QBrush(Qt::GlobalColor::darkRed);
            }
            if(number > 0.)
            {
                return QBrush(Qt::GlobalColor::darkGreen);
            }
        }
    }
    return ret;
}

void StocksModel::stocksUpdated()
{
    const auto newSize = stocks.size();
    if(size != 0 && (size == newSize))
    {
        emit dataChanged(createIndex(0, 0), createIndex((size = stocks.size()) - 1, COL_COUNT - 1));
    }else
    {
        if(size != 0)
        {
            beginRemoveRows(QModelIndex(), 0, size  - 1);
            endRemoveRows();
        }
        if((size = stocks.size()) != 0)
        {
            beginInsertRows(QModelIndex(), 0, (size = newSize) - 1);
            endInsertRows();
        }
    }
    emit time(stocks.getActualizationTime());
}

void StocksModel::stocksUpdated(size_t row)
{
    emit dataChanged(createIndex(row, 0), createIndex(row, COL_COUNT - 1));
}
