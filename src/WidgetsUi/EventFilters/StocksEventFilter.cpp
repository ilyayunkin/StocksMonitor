#include "StocksEventFilter.h"

#include <QEvent>
#include <QContextMenuEvent>
#include <QDebug>

#include <QTableView>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QAction>
#include <QInputDialog>

#include "WidgetsUi/ViewModels/StocksModel.h"
#include "WidgetsUi/ViewModels/StocksLimitsModel.h"

StocksEventFilter::StocksEventFilter(StocksInterface &stocksInterface,
                                     QTableView *table)
    : QObject(table)
    , table(table)
    , stocksInterface(stocksInterface)
    , menu(new QMenu)
    , portfolioAction(menu->addAction("To Portfolio"))
    , limitsAction(menu->addAction("Set limit"))
    , urlAction(menu->addAction("Open in the Internet"))
{
    table->installEventFilter(this);
    connect(table, &QAbstractItemView::doubleClicked,
            this, &StocksEventFilter::addLimit);
}

bool StocksEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::ContextMenu) {
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
                    stocksInterface.openUrl(ticker.data());
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

void StocksEventFilter::addLimit(const QModelIndex &sortModelIndex)
{
    qDebug() << __PRETTY_FUNCTION__;
    const auto row = static_cast<QSortFilterProxyModel *>(
                table->model())->mapToSource(sortModelIndex).row();
    Stock stock = this->stocksInterface.getStock(row);
    stocksInterface.addLimit(stock.ticker.data());
}
