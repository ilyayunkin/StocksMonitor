#include "StocksModel.h"

#include <QBrush>
#include <QDebug>
#include <QMimeData>

#include <algorithm>

StocksModel::StocksModel(StocksInterface &stocksInterface, QObject *parent) :
    QAbstractTableModel(parent),
    stocks(stocksInterface)
{
    qDebug() << __PRETTY_FUNCTION__ << __LINE__;
    stocksInterface.subscribeForChanges(this);
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
            Qt::ItemIsDragEnabled |
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
        }else{
            ret = section + 1;
        }
    }
    return ret;
}
QVariant StocksModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    int row = index.row();
    int col = index.column();
    assert(row >= 0);
    assert(col >= 0);

    const auto uRow = static_cast<size_t>(row);

    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if(uRow < (size = stocks.size()))
        {
            const Stock &stock = stocks.getStock(row);
            switch (col) {
            case NAME:
                ret = stock.name;
                break;
            case TICKER:
                ret = stock.ticker.data();
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
        if(uRow < (size = stocks.size()))
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

QString StocksModel::mimeType() const
{
    return "application/x-ticker";
}

QStringList StocksModel::mimeTypes() const
{
    return QStringList() << mimeType();
}

QMimeData *StocksModel::mimeData(const QModelIndexList &indexes) const
{
    if(indexes.size())
    {
        const Stock &stock = stocks.getStock(indexes.front().row());

        QMimeData *mimeData = new QMimeData;
        mimeData->setData(mimeType(), QByteArray(stock.ticker.data()));
        return mimeData;
    }

    return nullptr;
}

Qt::DropActions StocksModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
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
