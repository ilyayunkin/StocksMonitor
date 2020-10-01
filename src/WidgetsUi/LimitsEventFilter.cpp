#include "LimitsEventFilter.h"

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

#include "AbstractPocket.h"
#include "ModelsReference.h"

LimitsEventFilter::LimitsEventFilter(ModelsReference &limitsModel,
                                     AbstractPocket &pocket,
                                     QTableView *table) :
    QObject(table),
    table(table),
    models(limitsModel),
    pocket(pocket)
{
    table->installEventFilter(this);
    menu = new QMenu;
    pocketAction = new QAction("To Portfolio", this);
    urlAction = new QAction("Open in the Internet", this);
    menu->addAction(pocketAction);
    menu->addAction(urlAction);
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
                if(selected == pocketAction)
                {
                    auto quantity = QInputDialog::getInt(0, tr("Input quantity"), tr("Quantity"), 1, 1);
                    pocket.addStock(models.stocksModel->pluginName(), ticker, quantity);
                }else if(selected == urlAction)
                {
                    const auto stock = models.stocksModel->getStock(ticker);
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