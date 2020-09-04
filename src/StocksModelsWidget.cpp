#include "StocksModelsWidget.h"
#include "ui_StocksModelsWidget.h"

#include <QInputDialog>

#include <QTimer>

#include <StocksEventFilter.h>

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
        new StocksEventFilter(models.stocksModel->pluginName(), pocket, ui->stocksTableView);
    }
    {
        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(models.limitsModel.get());
        ui->stocksLimitsTableView->setModel(proxyModel);
        ui->stocksLimitsTableView->setSortingEnabled(true);
        ui->stocksLimitsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->stocksLimitsTableView->sortByColumn(StocksLimitsModel::DISTANCE, Qt::AscendingOrder);
    }

    connect(ui->stocksTableView, &QAbstractItemView::doubleClicked,
            [this](const QModelIndex &index)
    {
        qDebug() << __PRETTY_FUNCTION__;
        Stock stock = this->models.stocksModel->getStock(static_cast<QSortFilterProxyModel *>(ui->stocksTableView->model())->mapToSource(index).row());
        bool ok;
        float basePrice = QInputDialog::getDouble(this, stock.name, tr("Price"), stock.price, 0, 100000, 10, &ok);
        if(ok)
        {
            this->models.limitsModel->addStock(StockLimit{stock, basePrice});
        }
    });

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout,
            [this, &models]{this->ui->timeLabel->setText(models.time);});
    timer->start(1000);
}

StocksModelsWidget::~StocksModelsWidget()
{
    delete ui;
}
