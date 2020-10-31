#include "StocksModelsWidget.h"
#include "ui_StocksModelsWidget.h"

#include <QSortFilterProxyModel>

#include <QInputDialog>

#include "StocksEventFilter.h"
#include "LimitsEventFilter.h"
#include "StocksModel.h"
#include "StocksLimitsModel.h"

StocksModelsWidget::StocksModelsWidget(StocksInterface &stocksInterface,
                                       BuyRequestInterface &buyRequestInterface,
                                       QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StocksModelsWidget),
    stocksInterface(stocksInterface),
    buyRequestInterface(buyRequestInterface)
{
    ui->setupUi(this);
    {
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        StocksModel *model = new StocksModel(stocksInterface, this);
        proxyModel->setSourceModel(model);
        ui->stocksTableView->setModel(proxyModel);
        ui->stocksTableView->setSortingEnabled(true);
        ui->stocksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->stocksTableView->sortByColumn(StocksModel::NUM, Qt::AscendingOrder);
        new StocksEventFilter(stocksInterface, ui->stocksTableView);
        connect(model, &StocksModel::time,
                [this](std::string t){this->ui->timeLabel->setText(t.data());});
    }
    {
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        StocksLimitsModel *model = new StocksLimitsModel(buyRequestInterface, stocksInterface, this);
        proxyModel->setSourceModel(model);
        ui->stocksLimitsTableView->setModel(proxyModel);
        ui->stocksLimitsTableView->setSortingEnabled(true);
        ui->stocksLimitsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->stocksLimitsTableView->sortByColumn(StocksLimitsModel::DISTANCE, Qt::AscendingOrder);
        new LimitsEventFilter(stocksInterface, ui->stocksLimitsTableView);
    }
}

StocksModelsWidget::~StocksModelsWidget()
{
    delete ui;
}
