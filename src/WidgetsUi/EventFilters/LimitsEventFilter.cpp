#include "LimitsEventFilter.h"

#include <QEvent>
#include <QContextMenuEvent>
#include <QDebug>

#include <QTableView>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QKeyEvent>

#include "Application/BuyRequestInterface.h"
#include "WidgetsUi/ViewModels/StocksLimitsModel.h"

LimitsEventFilter::LimitsEventFilter(BuyRequestInterface &stocksInterface,
                                     QTableView *table)
    : QObject(table)
    , table(table)
    , buyRequestInterface(stocksInterface)
    , menu(new QMenu)
    , portfolioAction(menu->addAction("To Portfolio"))
    , urlAction(menu->addAction("Open in the Internet"))
{
    table->installEventFilter(this);
}

bool LimitsEventFilter::eventFilter(QObject *obj, QEvent *event)
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
                auto ticker = model->data(model->index(sortModelIndex.row(), StocksLimitsModel::TICKER)).toByteArray();
                auto selected = menu->exec(globalPos);
                if(selected == portfolioAction)
                {
                    auto quantity = QInputDialog::getInt(0, tr("Input quantity"), tr("Quantity"), 1, 1);
                    buyRequestInterface.addToPortfolio(ticker, quantity);
                }else if(selected == urlAction)
                {
                    buyRequestInterface.openUrl(ticker.data());
                }
            }
        }
    }
        break;
    case QEvent::KeyPress:
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete)
        {
            auto model = table->model();
            if(model)
            {
                auto index = table->currentIndex();
                if(index.isValid())
                {
                    auto ticker = model->data(model->index(index.row(), StocksLimitsModel::TICKER)).toByteArray();
                    buyRequestInterface.remove(ticker);
                }
            }
        }
    }
        break;
    default:
        break;
    }
    return QObject::eventFilter(obj, event);
}
