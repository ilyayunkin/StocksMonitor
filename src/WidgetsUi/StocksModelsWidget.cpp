#include "StocksModelsWidget.h"
#include "ui_StocksModelsWidget.h"

#include <QTimer>
#include <QSortFilterProxyModel>

#include <QInputDialog>

#include "Rules/AbstractStocksModel.h"
#include "Rules/ModelsReference.h"

#include "StocksEventFilter.h"
#include "LimitsEventFilter.h"
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
        StocksModel *model = new StocksModel(*models.stocksModel.get(), this);
        models.stocksModel.get()->setView(model);
        proxyModel->setSourceModel(model);
        ui->stocksTableView->setModel(proxyModel);
        ui->stocksTableView->setSortingEnabled(true);
        ui->stocksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->stocksTableView->sortByColumn(StocksModel::NUM, Qt::AscendingOrder);
        new StocksEventFilter(models, pocket, ui->stocksTableView);
    }
    {
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        StocksLimitsModel *model = new StocksLimitsModel(*models.limitsModel.get(), this);
        models.limitsModel.get()->setView(model);
        proxyModel->setSourceModel(model);
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
