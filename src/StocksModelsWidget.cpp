#include "StocksModelsWidget.h"
#include "ui_StocksModelsWidget.h"

#include <QInputDialog>

StocksModelsWidget::StocksModelsWidget(ModelsReference models, QWidget *parent) :
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
}

StocksModelsWidget::~StocksModelsWidget()
{
    delete ui;
}
