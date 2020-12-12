#include "StocksModelsWidget.h"
#include "ui_StocksModelsWidget.h"

#include <QSortFilterProxyModel>

#include <QInputDialog>

#include "EventFilters/StocksEventFilter.h"
#include "EventFilters/LimitsEventFilter.h"
#include "ViewModels/StocksModel.h"
#include "ViewModels/StocksLimitsModel.h"

StocksModelsWidget::StocksModelsWidget(ViewInterfacesPair &viewInterfaces,
                                       QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StocksModelsWidget)
    , viewInterfaces(viewInterfaces)
    , stocksInterface(viewInterfaces.stocksInterface)
    , buyRequestInterface(viewInterfaces.buyRequestInterface)
{
    ui->setupUi(this);
    {
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        StocksModel *model = new StocksModel(stocksInterface, this);
        proxyModel->setSourceModel(model);
        ui->stocksTableView->setModel(proxyModel);
        ui->stocksTableView->setDragDropMode(QAbstractItemView::DragOnly);
        ui->stocksTableView->setSortingEnabled(true);
        ui->stocksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->stocksTableView->sortByColumn(StocksModel::DERIVATION, Qt::AscendingOrder);
        new StocksEventFilter(stocksInterface, ui->stocksTableView);
        connect(model, &StocksModel::time,
                [this](TimeString t){this->ui->timeLabel->setText(t.data());});
    }
    {
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        StocksLimitsModel *model = new StocksLimitsModel(buyRequestInterface, stocksInterface, this);
        proxyModel->setSourceModel(model);
        ui->stocksLimitsTableView->setModel(proxyModel);
        ui->stocksLimitsTableView->setDragDropMode(QAbstractItemView::DropOnly);
        ui->stocksLimitsTableView->setSortingEnabled(true);
        ui->stocksLimitsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->stocksLimitsTableView->sortByColumn(StocksLimitsModel::DISTANCE, Qt::AscendingOrder);
        new LimitsEventFilter(buyRequestInterface, ui->stocksLimitsTableView);
    }
    ui->sourceLinkLabel->setText(tr("Original resource: ") + viewInterfaces.url);
}

StocksModelsWidget::~StocksModelsWidget()
{
    delete ui;
}
