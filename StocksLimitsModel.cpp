#include "StocksLimitsModel.h"

#include <QBrush>
#include <QTimer>

StocksLimitsModel::StocksLimitsModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    stockLimits.reserve(100);
    {
        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout, this, &StocksLimitsModel::update);
        t->start(5000);
    }
}

void StocksLimitsModel::update()
{
    if(stocksModel != nullptr)
    {
        int i = 0;
        for(auto &limit : stockLimits)
        {
            float price = stocksModel->getStockPrice(limit.ticker);
            if(price != limit.price)
            {
                limit.price = price;
                emit dataChanged(createIndex(i, 0), createIndex(i, COL_COUNT - 1));
            }
            ++i;
        }
    }
}

int StocksLimitsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return stockLimits.size();
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
        if(row < stockLimits.size())
        {
            const StockLimit &stock = stockLimits.at(row);
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
        if(row < stockLimits.size())
        {
            const StockLimit &stock = stockLimits.at(row);
            float number =
                    (stock.price - stock.basePrice) / stock.basePrice;
            if(number < 0.01)
            {
                return QBrush(Qt::GlobalColor::red);
            }else if(number < 0.05)
            {
                return QBrush(Qt::GlobalColor::yellow);
            }else if(number < 0.1)
            {
                return QBrush(Qt::GlobalColor::green);
            }
        }
    }
    return ret;
}

void StocksLimitsModel::setStocksModel(AbstractStocksModel *stocksModel)
{
    this->stocksModel = stocksModel;
}

bool StocksLimitsModel::setData(const QModelIndex &index,
                                const QVariant &value,
                                int role)
{
    int row = index.row();
    int col = index.column();

    if ((role == Qt::EditRole) && (col == BASE_PRICE) && (row < stockLimits.size()))
    {
        float v = value.toFloat();
        stockLimits[row].basePrice = v;
        return true;
    }
    return false;
}

void StocksLimitsModel::addStock(const StockLimit &stockLimit)
{
    auto size = stockLimits.size();

    beginInsertRows(QModelIndex(), size, size);

    stockLimits.push_back(stockLimit);

    endInsertRows();
}

