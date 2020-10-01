#include "StocksModelsWidget.h"
#include "ui_StocksModelsWidget.h"

#include <QTimer>
#include <QSortFilterProxyModel>

#include <QInputDialog>

#include "StocksEventFilter.h"
#include "LimitsEventFilter.h"
#include "ModelsReference.h"
#include "StocksModel.h"
#include "StocksLimitsModel.h"

StocksModelsWidget::StocksModelsWidget(ModelsReference &models, AbstractPocket &pocket, QWidget *parent) :
    QWidget(parent),
    models(models),
    ui(new Ui::StocksModelsWidget)
{
    ui->setupUi(this);
    {
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(models.stocksModel.get());
        ui->stocksTableView->setModel(proxyModel);
        ui->stocksTableView->setSortingEnabled(true);
        ui->stocksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->stocksTableView->sortByColumn(StocksModel::NUM, Qt::AscendingOrder);
        new StocksEventFilter(models, pocket, ui->stocksTableView);
    }
    {
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(models.limitsModel.get());
        ui->stocksLimitsTableView->setModel(proxyModel);
        ui->stocksLimitsTableView->setSortingEnabled(true);
        ui->stocksLimitsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->stocksLimitsTableView->sortByColumn(StocksLimitsModel::DISTANCE, Qt::AscendingOrder);
        new LimitsEventFilter(models, pocket, ui->stocksLimitsTableView);
    }

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            [this, &models]{this->ui->timeLabel->setText(models.time);});
    timer->start(1000);
}

StocksModelsWidget::~StocksModelsWidget()
{
    delete ui;
}
