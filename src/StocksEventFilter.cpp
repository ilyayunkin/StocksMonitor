#include "StocksEventFilter.h"

#include <QEvent>
#include <QContextMenuEvent>
#include <QDebug>

#include <QTableView>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QAction>
#include <QInputDialog>

#include <StocksModel.h>

#include "AbstractPocket.h"
#include "ModelsReference.h"

StocksEventFilter::StocksEventFilter(ModelsReference &limitsModel,
                                     AbstractPocket &pocket,
                                     QTableView *table) :
    QObject(table),
    table(table),
    models(limitsModel),
    pocket(pocket)
{
    table->installEventFilter(this);
    menu = new QMenu;
    pocketAction = new QAction("To pocket", this);
    limitsAction = new QAction("Set limit", this);
    menu->addAction(pocketAction);
    menu->addAction(limitsAction);

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
                if(selected == pocketAction)
                {
                    auto quantity = QInputDialog::getInt(0, tr("Input quantity"), tr("Quantity"), 1, 1);
                    pocket.addStock(models.stocksModel->pluginName(), ticker, quantity);
                }else if(selected == limitsAction)
                {
                    addLimit(sortModelIndex);
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
    Stock stock = this->models.stocksModel->getStock(
                static_cast<QSortFilterProxyModel *>(
                    table->model())->mapToSource(sortModelIndex).row());
    bool ok;
    float basePrice = QInputDialog::getDouble(table,
                                              stock.name,
                                              tr("Price"),
                                              stock.price,
                                              0, 100000, 10, &ok);
    if(ok)
    {
        this->models.limitsModel->addStock(StockLimit{stock, basePrice});
    }
}
