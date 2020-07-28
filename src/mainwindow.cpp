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
#include <QSettings>

#include "PopUpWindow.h"
#include "logger.h"
#include "StoryWidget.h"
#include "SoundDialog.h"

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
        {
            QMenu *fileMenu = bar->addMenu(tr("File"));
            {
                QAction *saveAction = fileMenu->addAction(tr("Save limits"));
                connect(saveAction, &QAction::triggered, this, &MainWindow::save);
            }
        }
        {
            QMenu *ToolsMenu = bar->addMenu(tr("Tools"));
            {
                QAction *storyAction = ToolsMenu->addAction(tr("Show history"));
                connect(storyAction, &QAction::triggered, &StoryWidget::showStory);
            }
            {
                QAction *setupSoundAction = ToolsMenu->addAction(tr("Setup sound"));
                connect(setupSoundAction, &QAction::triggered, this, &MainWindow::selectSoundFile);
            }
        }
    }

    {
        QSettings settings;
        QString filename = settings.value("sound").toString();
        qDebug() << filename;
        setupFile(filename);
    }

    connect(stocksTableView, &QAbstractItemView::doubleClicked,
            this, &MainWindow::stockDoubleClicked);

    connect(&limitsModel, &StocksLimitsModel::boundCrossed,
            this, &MainWindow::signalize);

    connect(&limitsModel, &StocksLimitsModel::crossedLimit,
            this, &MainWindow::crossedLimit);
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

void MainWindow::selectSoundFile()
{
    QString filename = SoundDialog::getSoundFileName();
    qDebug() << filename;
    setupFile(filename);
    if(!filename.isEmpty())
    {
        QSettings settings;
        settings.setValue("sound", filename);
    }
}

void MainWindow::setupFile(const QString &filename)
{
    if(!filename.isEmpty()){
        sound = std::make_shared<QSound>(filename);
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
    if(sound.get() != nullptr){
        sound->play();
    }
}

void MainWindow::lastTime(const QByteArray &time)
{
    statusLabel->setText(QString(time));
    limitsModel.update();
}

void MainWindow::crossedLimit(const StockLimit &stockLimit)
{
    QString logMessage = QString("%1\n%2")
            .arg(stockLimit.name)
            .arg(stockLimit.price);
    Logger::instance().log(logMessage);
    PopUpWindow::showPopUpWindow(logMessage);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QApplication::quit();
}
