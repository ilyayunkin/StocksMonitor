#include "PortfolioModel.h"

#include <QMessageBox>

#include <QDebug>

#include "ExceptionClasses.h"
#include "Presenters/StockHint.h"
#include "Application/PortfolioInterface.h"

void PortfolioModel::stocksUpdated()
{    
    const auto newSize = portfolioInterface.size();
    if(size != 0 && (size == newSize))
    {
        emit dataChanged(createIndex(0, 0), createIndex((size = portfolioInterface.size()) - 1, COL_COUNT - 1));
    }else
    {
        if(size != 0)
        {
            beginRemoveRows(QModelIndex(), 0, size  - 1);
            endRemoveRows();
        }
        if((size = portfolioInterface.size()) != 0)
        {
            beginInsertRows(QModelIndex(), 0, (size = newSize) - 1);
            endInsertRows();
        }
    }
    emit updated();
}

void PortfolioModel::stocksUpdated(size_t row)
{
    emit dataChanged(createIndex(row, 0),
                     createIndex(row, COL_COUNT - 1));
    emit updated();
}

PortfolioModel::PortfolioModel(PortfolioInterface &portfolioInterface, QObject *parent) :
    QAbstractTableModel(parent),
    portfolioInterface(portfolioInterface)
{
    portfolioInterface.subscribeForChanges(this);
}

int PortfolioModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return size = portfolioInterface.size();
}

int PortfolioModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return COL_COUNT;
}

Qt::ItemFlags PortfolioModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags ret =QAbstractTableModel::flags(index) |
            Qt::ItemIsEnabled |
            Qt::ItemIsSelectable;
    if(index.column() == QUANTITY ||
            index.column() == SELL_PRICE)
    {
        ret |= Qt::ItemIsEditable;
    }

    return ret;
}

QVariant PortfolioModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            case QUANTITY:
                ret = tr("Quantity");
                break;
            case TICKER:
                ret = tr("TICKER");
                break;
            case PRICE_BASE_CURRENCY:
                ret = tr("Price");
                break;
            case CURRENCY:
                ret = tr("Currency");
                break;
            case SUM:
                ret = tr("Sum");
                break;
            case SELL_PRICE:
                ret = tr("Sell price");
                break;
            default:
                break;
            }
        }
    }
    return ret;
}

QVariant PortfolioModel::data(const QModelIndex &index, int role) const
{
    QVariant ret;
    const int row = index.row();
    const int col = index.column();
    assert(row >= 0);
    assert(col >= 0);

    const auto uRow = static_cast<size_t>(row);

    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if(uRow < (size = portfolioInterface.size()))
        {
            const PortfolioEntry &entry = portfolioInterface.getPortfolioEntry(row);
            switch (col) {
            case NAME:
                ret = entry.name;
                break;
            case TICKER:
                ret = entry.ticker.data();
                break;
            case QUANTITY:
                ret = entry.quantity;
                break;
            case PRICE_BASE_CURRENCY:
                ret = entry.price;
                break;
            case CURRENCY:
                ret = entry.currency.data();
                break;
            case SUM:
                ret = entry.sum;
                break;
            case SELL_PRICE:
                ret = entry.sellPrice;
                break;
            default:
                break;
            }
        }
    }
    if (role == Qt::ToolTipRole)
    {
        if(uRow < (size = portfolioInterface.size()))
        {
            auto stock = portfolioInterface.getStock(row);
            ret = StockHint::getHint(stock);
        }
    }
    if (role == Qt::BackgroundRole)
    {
        if(uRow < (size = portfolioInterface.size()))
        {
            const PortfolioEntry &entry = portfolioInterface.getPortfolioEntry(row);
            if(entry.sellPrice > 0
                    && entry.price >= entry.sellPrice)
            {
                ret = QBrush(Qt::red);
            }
        }
    }
    return ret;
}

bool PortfolioModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int col = index.column();
    assert(row >= 0);
    assert(col >= 0);

    const auto uRow = static_cast<size_t>(row);

    if ((role == Qt::EditRole) && (uRow < (size =portfolioInterface.size())))
    {
        if(col == QUANTITY)
        {
            int quantity = value.toInt();
            if(quantity > 0)
            {
                portfolioInterface.setPortfolioEntryQuantity(row, quantity);
                emit dataChanged(createIndex(row, 0),
                                 createIndex(row, COL_COUNT - 1));
            }else
            {
                const PortfolioEntry entry = portfolioInterface.getPortfolioEntry(row);
                bool answer =
                        QMessageBox::question(0,
                                              tr("Delete?"),
                                              tr("You've selected 0 items:\n %1")
                                              .arg(QString(entry.ticker.data())))
                        == QMessageBox::Yes;
                if(answer)
                {
                    beginRemoveRows(QModelIndex(), row, row);
                    portfolioInterface.deletePortfolioEntry(row);
                    endRemoveRows();
                }
            }
            return true;
        }else if(col == SELL_PRICE)
        {
            double sellPrice = value.toDouble();
            portfolioInterface.setPortfolioEntryReferencePrice(row, sellPrice);
            emit dataChanged(createIndex(row, 0),
                             createIndex(row, COL_COUNT - 1));
        }
    }
    return false;
}
