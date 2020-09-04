#include "StocksEventFilter.h"

#include <QEvent>
#include <QContextMenuEvent>
#include <QDebug>

#include <QTableView>
#include <QMenu>
#include <QAction>
#include <QInputDialog>

#include <StocksModel.h>

#include "AbstractPocket.h"

StocksEventFilter::StocksEventFilter(QString plugin, AbstractPocket &pocket, QTableView *table) :
    QObject(table),
    table(table),
    plugin(plugin),
    pocket(pocket)
{
    table->installEventFilter(this);
    menu = new QMenu;
    pocketAction = new QAction("To pocket", this);
    menu->addAction(pocketAction);
}

bool StocksEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::ContextMenu) {
        auto model = table->model();
        if(model)
        {
            QContextMenuEvent *contextEvent = static_cast<QContextMenuEvent*>(event);
            auto globalPos = contextEvent->globalPos();
            auto index = table->indexAt(table->viewport()->mapFromGlobal(globalPos));

            if(index.isValid())
            {
                auto ticker = model->data(model->index(index.row(), StocksModel::TICKER)).toByteArray();
                auto selected = menu->exec(globalPos);
                if(selected == pocketAction){
                    auto quantity = QInputDialog::getInt(0, tr("Input quantity"), tr("Quantity"), 1, 1);
                    pocket.addStock(plugin, ticker, quantity);
                }
            }
        }
    }

    return QObject::eventFilter(obj, event);
}
