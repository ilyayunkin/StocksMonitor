#include "StocksModel.h"

#include <QBrush>

#include <algorithm>

StocksModel::StocksModel(const QString &plugin, const QByteArray &currencyCode, QObject *parent) :
    QAbstractTableModel(parent),
    _pluginName(plugin),
    _currencyCode(currencyCode)
{
}

QString StocksModel::pluginName() const
{
    return _pluginName;
}

QByteArray StocksModel::currencyCode() const
{
    return _currencyCode;
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
    if (role == Qt::ForegroundRole)
    {
        int row = index.row();
        int col = index.column();
        if(row < stocks.size())
        {
            const Stock &stock = stocks.at(row);
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

void StocksModel::setStocks(StocksList &&stocks)
{
    if(!this->stocks.empty() && (stocks.size() == this->stocks.size()))
    {
        this->stocks = stocks;
        emit dataChanged(createIndex(0, 0), createIndex(stocks.size() - 1, COL_COUNT - 1));
    }else
    {
        if(!this->stocks.empty())
        {
            beginRemoveRows(QModelIndex(), 0, this->stocks.size() - 1);
            endRemoveRows();
        }
        if(!stocks.empty())
        {
            beginInsertRows(QModelIndex(), 0, stocks.size() - 1);
            this->stocks = stocks;
            endInsertRows();
        }
    }
}

Stock StocksModel::getStock(const QByteArray &ticker) const
{
    Stock ret{};
    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        ret = *cit;
    }
    return ret;
}

Stock StocksModel::getStock(const size_t i) const
{
    Stock ret{};
    if(i < stocks.size())
    {
        ret = stocks.at(i);
    }
    return ret;
}

float StocksModel::getStockPrice(const QByteArray &ticker) const
{
    float ret{};
    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        ret = cit->price;
    }
    return ret;
}
