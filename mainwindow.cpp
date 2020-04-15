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
        stocksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    {
        stocksLimitsTableView = new QTableView;
        hlay->addWidget(stocksLimitsTableView);
        stocksLimitsTableView->setModel(&limitsModel);
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
    Stock stock = model.getStock(index.row());
    bool ok;
    float basePrice = QInputDialog::getDouble(this, tr("Input base price"), tr("Price"), stock.price, 0, 100000, 10, &ok);
    if(ok)
    {
        limitsModel.addStock(StockLimit{stock, basePrice});
    }
}
