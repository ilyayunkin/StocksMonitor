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
#include "PocketWidget.h"
#include "PortfolioModel.h"
#include "StocksLimitsModel.h"
#include "StocksModel.h"
#include "Sounds/Signalizer.h"

MainWindow::MainWindow(Application &application,
                       QWidget *parent)
    : QMainWindow(parent),
      application(application)
{
    QWidget *w = new QWidget;
    setCentralWidget(w);

    {
        QVBoxLayout *vlay = new QVBoxLayout(w);
        {
            QHBoxLayout *hlay = new QHBoxLayout;
            vlay->addLayout(hlay);
            {
                QTabWidget *tabWidget = new QTabWidget;
                hlay->addWidget(tabWidget);
                for(auto &modelsRef : application.modelsReferences())
                {
                    StocksModelsWidget *w = new StocksModelsWidget(modelsRef, *(application.getPortfolio()));
                    QHBoxLayout *viewLay = new QHBoxLayout;
                    viewLay->setMargin(0);

                    tabWidget->addTab(w, modelsRef.name);
                }
                {
                    PocketWidget *pocketWidget = new PocketWidget(application.getPortfolio(), application);
                    tabWidget->addTab(pocketWidget, QIcon("://img/portfolio.png"), tr("Portfolio"));
                }
                for(auto &ref : application.modelsReferences())
                {
                    QObject::connect(ref.limitsModel.get(), &StocksLimitsModel::boundCrossed,
                                     [this](){QApplication::alert(this);});
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
                        [this](){this->application.getSignalizer()->changeSound();});
            }
        }
    }
}

MainWindow::~MainWindow()
{
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
