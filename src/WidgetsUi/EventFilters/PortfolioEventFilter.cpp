#include "PortfolioEventFilter.h"

#include <QEvent>
#include <QContextMenuEvent>
#include <QDebug>

#include <QTableView>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QAction>

#include "WidgetsUi/ViewModels/PortfolioModel.h"
#include "Application/PortfolioInterface.h"

PortfolioEventFilter::PortfolioEventFilter(PortfolioInterface &portfolioInterface,
                                           QTableView *table)
    : QObject(table)
    , table(table)
    , portfolioInterface(portfolioInterface)
    , menu(new QMenu)
    , urlAction(menu->addAction("Open in the Internet"))
{
    table->installEventFilter(this);
}

bool PortfolioEventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::ContextMenu)
    {
        auto model = table->model();
        if(model)
        {
            const auto contextEvent = static_cast<QContextMenuEvent*>(event);
            const auto globalPos = contextEvent->globalPos();
            const auto sortModelIndex = table->indexAt(table->viewport()->mapFromGlobal(globalPos));

            if(sortModelIndex.isValid())
            {
                QAbstractProxyModel *modelAsProxy = dynamic_cast<QAbstractProxyModel *>(model);
                const auto originalIndex = modelAsProxy->mapToSource(sortModelIndex);
                auto selected = menu->exec(globalPos);
                if(selected == urlAction)
                {
                    portfolioInterface.openUrl(originalIndex.row());
                }
            }
        }
    }
    return QObject::eventFilter(obj, event);
}
