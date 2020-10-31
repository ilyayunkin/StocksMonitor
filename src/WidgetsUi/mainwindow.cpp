#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QFileDialog>
#include <QMenuBar>
#include <QTabWidget>

#include <QFile>
#include <QTextStream>
#include <QApplication>

#include "StoryWidget.h"
#include "StocksModelsWidget.h"
#include "PortfolioWidget.h"
#include "PortfolioModel.h"
#include "StocksLimitsModel.h"
#include "StocksModel.h"
#include "Sounds/Signalizer.h"
#include "StatisticsWidget.h"

MainWindow::MainWindow(Application &application,
                       QWidget *parent)
    : QMainWindow(parent),
      application(application)
{
    QWidget *w = new QWidget;
    signalizer = new Signalizer;
    notifier = new Notifier(this, signalizer);
    application.setNotifier(notifier);
    setCentralWidget(w);

    {
        QVBoxLayout *vlay = new QVBoxLayout(w);
        {
            QHBoxLayout *hlay = new QHBoxLayout;
            vlay->addLayout(hlay);
            {
                QTabWidget *tabWidget = new QTabWidget;
                hlay->addWidget(tabWidget);
                for(auto &modelsRef : application.getViewInterfaces())
                {
                    StocksModelsWidget *w =
                            new StocksModelsWidget(modelsRef.stocksInterfaces,
                                                   modelsRef.buyRequestInterfaces);
                    QHBoxLayout *viewLay = new QHBoxLayout;
                    viewLay->setMargin(0);

                    tabWidget->addTab(w, modelsRef.name);
                }
                {
                    auto portfolioModel = new PortfolioModel(application.getPortfolioInterface(), this);
                    auto *portfolioWidget =
                            new PortfolioWidget(portfolioModel, application);
                    tabWidget->addTab(portfolioWidget, QIcon("://img/portfolio.png"), tr("Portfolio"));
                }
                {
                    auto *statisticsWidget = new StatisticsWidget(application, this);
                    tabWidget->addTab(statisticsWidget, tr("Statistics"));
                }
            }
        }
    }
    {
        QMenuBar *bar = new QMenuBar;
        setMenuBar(bar);
        //        {
        //            QMenu *fileMenu = bar->addMenu(tr("File"));
        //            {
        //                QAction *saveAction = fileMenu->addAction(tr("Save limits"));
        //                connect(saveAction, &QAction::triggered, this, &MainWindow::save);
        //            }
        //        }
        {
            QMenu *ToolsMenu = bar->addMenu(tr("Tools"));
            {
                QAction *storyAction = ToolsMenu->addAction(tr("Show history"));
                connect(storyAction, &QAction::triggered, &StoryWidget::showStory);
            }
            {
                QAction *setupSoundAction = ToolsMenu->addAction(tr("Setup sound"));
                connect(setupSoundAction, &QAction::triggered,
                        [this](){signalizer->changeSound();});
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete notifier;
    delete signalizer;
}

void MainWindow::save()
{
#if 0
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
#endif
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    QApplication::quit();
}
