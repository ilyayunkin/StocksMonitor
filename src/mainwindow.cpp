#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QHeaderView>
#include <QDebug>
#include <QInputDialog>
#include <QMenuBar>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QApplication>

#include "PopUpWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      limitsModel(false)
{
    QWidget *w = new QWidget;
    setCentralWidget(w);

    {
        QVBoxLayout *vlay = new QVBoxLayout(w);
        {
            QHBoxLayout *hlay = new QHBoxLayout;
            vlay->addLayout(hlay);
            {
                stocksTableView = new QTableView;
                hlay->addWidget(stocksTableView);
                stocksTableView->setModel(&model);

                QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
                proxyModel->setSourceModel(&model);
                stocksTableView->setModel(proxyModel);
                stocksTableView->setSortingEnabled(true);
                stocksTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
                stocksTableView->sortByColumn(StocksModel::NUM, Qt::AscendingOrder);
            }
            {
                stocksLimitsTableView = new QTableView;
                hlay->addWidget(stocksLimitsTableView);

                QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
                proxyModel->setSourceModel(&limitsModel);
                stocksLimitsTableView->setModel(proxyModel);
                stocksLimitsTableView->setSortingEnabled(true);
                stocksLimitsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
                stocksLimitsTableView->sortByColumn(StocksLimitsModel::DISTANCE, Qt::AscendingOrder);
            }
        }
        {
            statusLabel = new QLabel;
            vlay->addWidget(statusLabel);
        }
    }
    limitsModel.setStocksModel(&model);
    {
        QMenuBar *bar = new QMenuBar;
        setMenuBar(bar);
        QMenu *fileMenu = bar->addMenu(tr("File"));
        QAction *saveAction = fileMenu->addAction(tr("Save limits"));
        connect(saveAction, &QAction::triggered, this, &MainWindow::save);
    }

    connect(stocksTableView, &QAbstractItemView::doubleClicked,
            this, &MainWindow::stockDoubleClicked);

    connect(&limitsModel, &StocksLimitsModel::boundCrossed,
            this, &MainWindow::signalize);

    connect(&limitsModel, &StocksLimitsModel::crossedLimit,
            this, &MainWindow::crossedLimit);

    speaker.setRate(0.3);
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

void MainWindow::save()
{
    qDebug() << __PRETTY_FUNCTION__;
    QString filename = QFileDialog::getSaveFileName(this, tr("Save"));
    if(!filename.isNull())
    {
        QFile f(filename);
        if(f.open(QIODevice::WriteOnly))
        {
            QTextStream stream(&f);
            StockLimitsList list = limitsModel.getList();
            int i = 0;
            for(auto &limit : list)
            {
                if(i > 0)
                {
                    stream << '\n';
                }
                stream << limit.name << '\t'
                       << limit.ticker << '\t'
                       << limit.basePrice;
                ++i;
            }
            stream.flush();
            f.close();
        }
    }
}

void MainWindow::signalize()
{
    QApplication::alert(this);
    speaker.say("Woop woop! Woop woop! Woop woop!");
}

void MainWindow::lastTime(const QByteArray &time)
{
    statusLabel->setText(QString(time));
    limitsModel.update();
}

void MainWindow::crossedLimit(const StockLimit &stockLimit)
{
    PopUpWindow::showPopUpWindow(QString("%1\n%2")
                                 .arg(stockLimit.name)
                                 .arg(stockLimit.price));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QApplication::quit();
}
