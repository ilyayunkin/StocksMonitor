#include "mainwindow.h"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *w = new QWidget;
    setCentralWidget(w);

    QHBoxLayout *hlay = new QHBoxLayout(w);
    {
        stocksTableView = new QTableView;
        hlay->addWidget(stocksTableView);
        stocksTableView->setModel(&model);

        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(&model);
        stocksTableView->setModel(proxyModel);
        stocksTableView->setSortingEnabled(true);
        stocksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        proxyModel->sort(StocksModel::NUM, Qt::AscendingOrder);
    }
    {
        stocksLimitsTableView = new QTableView;
        hlay->addWidget(stocksLimitsTableView);

        QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(&limitsModel);
        stocksLimitsTableView->setModel(proxyModel);
        stocksLimitsTableView->setSortingEnabled(true);
        stocksLimitsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        proxyModel->sort(StocksLimitsModel::DISTANCE, Qt::AscendingOrder);
    }
    limitsModel.setStocksModel(&model);

    connect(stocksTableView, &QAbstractItemView::doubleClicked,
            this, &MainWindow::stockDoubleClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::stockDoubleClicked(const QModelIndex &index)
{
    qDebug() << __PRETTY_FUNCTION__;
    Stock stock = model.getStock(static_cast<QSortFilterProxyModel *>(stocksTableView->model())->mapToSource(index).row());
    bool ok;
    float basePrice = QInputDialog::getDouble(this, stock.name, tr("Price"), stock.price, 0, 100000, 10, &ok);
    if(ok)
    {
        limitsModel.addStock(StockLimit{stock, basePrice});
    }
}
