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
    if (role == Qt::DisplayRole){
        if (orientation == Qt::Horizontal){
            if((section >= NAME) && (section < COL_COUNT)){
                static const QString headers[] ={
                    tr("Name")
                    , tr("TICKER")
                    , tr("Price")
                    , tr("%\nDay")
                    , tr("%\nWeek")
                    , tr("%\nMonth")
                    , tr("%\nYear")
                };
                return headers[section];
            }
        }else{
            return  section + 1;
        }
    }
    return QVariant();
}
QVariant StocksModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    assert(row >= 0);
    assert(col >= 0);

    const auto uRow = static_cast<size_t>(row);

    if ((role == Qt::DisplayRole) || (role == Qt::EditRole)){
        if(uRow < (size = stocks.size())){
            const Stock &stock = stocks.getStock(row);
            switch (col) {
            case NAME: return stock.name;
            case TICKER: return stock.ticker.data();
            case PRICE: return stock.price;
            case DERIVATION: return stock.derivation;
            case DERIVATION_WEEK: return stock.derivationWeek;
            case DERIVATION_MONTH: return stock.derivationMonth;
            case DERIVATION_YEAR: return stock.derivationYear;
            default: break;
            }
        }
    }else if (role == Qt::ForegroundRole){
        if(uRow < (size = stocks.size())){
            const Stock &stock = stocks.getStock(row);
            float number = [&]{
                switch (col) {
                case DERIVATION: return stock.derivation;
                case DERIVATION_WEEK: return stock.derivationWeek;
                case DERIVATION_MONTH: return stock.derivationMonth;
                case DERIVATION_YEAR: return stock.derivationYear;
                default: return 0.f;
                }}();
            if(number < 0.){
                return QBrush(Qt::GlobalColor::darkRed);
            }else if(number > 0.){
                return QBrush(Qt::GlobalColor::darkGreen);
            }
        }
    }
    return QVariant();
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
