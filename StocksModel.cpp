#include "StocksModel.h"


StocksModel::StocksModel(QObject *parent) :
    QAbstractTableModel(parent)
{
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
                ret = tr("Derivation");
                break;
            case DERIVATION_WEEK:
                ret = tr("Derivation Week");
                break;
            case DERIVATION_MONTH:
                ret = tr("Derivation Month");
                break;
            case DERIVATION_YEAR:
                ret = tr("Derivation Year");
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
        if(row < stocks.size())
        {
            const Stock &stock = stocks.at(row);
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
    return ret;
}

void StocksModel::setStocks(StocksList &&stocks)
{
    if(!this->stocks.isEmpty())
    {
        beginRemoveRows(QModelIndex(), 0, this->stocks.size() - 1);
        endRemoveRows();
    }
    if(!stocks.isEmpty())
    {
        beginInsertRows(QModelIndex(), 0, stocks.size() - 1);
        this->stocks = stocks;
        endInsertRows();
    }
}
