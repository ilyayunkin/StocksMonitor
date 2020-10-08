#include "StocksEventFilter.h"

#include <QEvent>
#include <QContextMenuEvent>
#include <QDebug>

#include <QTableView>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QDesktopServices>

#include "StocksModel.h"
#include "StocksLimitsModel.h"

StocksEventFilter::StocksEventFilter(StocksInterface &stocksInterface,
                                     QTableView *table) :
    QObject(table),
    table(table),
    stocksInterface(stocksInterface)
{
    table->installEventFilter(this);
    menu = new QMenu;
    portfolioAction = new QAction("To Portfolio", this);
    limitsAction = new QAction("Set limit", this);
    urlAction = new QAction("Open in the Internet", this);
    menu->addAction(portfolioAction);
    menu->addAction(limitsAction);
    menu->addAction(urlAction);

    connect(table, &QAbstractItemView::doubleClicked,
            this, &StocksEventFilter::addLimit);
}

bool StocksEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type()) {
    case QEvent::ContextMenu:
    {
        auto model = table->model();
        if(model)
        {
            auto contextEvent = static_cast<QContextMenuEvent*>(event);
            auto globalPos = contextEvent->globalPos();
            auto sortModelIndex = table->indexAt(table->viewport()->mapFromGlobal(globalPos));

            if(sortModelIndex.isValid())
            {
                auto ticker = model->data(model->index(sortModelIndex.row(), StocksModel::TICKER)).toByteArray();
                auto selected = menu->exec(globalPos);
                if(selected == portfolioAction)
                {
                    auto quantity = QInputDialog::getInt(0, tr("Input quantity"), tr("Quantity"), 1, 1);
                    stocksInterface.addToPortfolio(ticker, quantity);
                }else if(selected == limitsAction)
                {
                    addLimit(sortModelIndex);
                }else if(selected == urlAction)
                {
                    const auto stock = stocksInterface.getStock(ticker);
                    if(!stock.url.isEmpty())
                    {
                        QDesktopServices::openUrl(QUrl(stock.url));
                    }
                }
            }
        }
    }
        break;
#if 0
    case QEvent::MouseButtonDblClick:
    {
        qDebug() << __PRETTY_FUNCTION__;
        auto model = table->model();
        if(model)
        {
            auto contextEvent = static_cast<QContextMenuEvent*>(event);
            auto globalPos = contextEvent->globalPos();
            auto sortModelIndex = table->indexAt(table->viewport()->mapFromGlobal(globalPos));
            if(sortModelIndex.isValid())
            {
                addLimit(sortModelIndex);
            }
        }

    }
        break;
#endif
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}

void StocksEventFilter::addLimit(const QModelIndex &sortModelIndex)
{
    qDebug() << __PRETTY_FUNCTION__;
    const auto row = static_cast<QSortFilterProxyModel *>(
                table->model())->mapToSource(sortModelIndex).row();
    Stock stock = this->stocksInterface.getStock(row);
    bool ok;
    float basePrice = QInputDialog::getDouble(table,
                                              stock.name,
                                              tr("Price"),
                                              stock.price,
                                              0, 100000, 10, &ok);
    if(ok)
    {
        stocksInterface.addLimit(stock.ticker, basePrice);
    }
}
