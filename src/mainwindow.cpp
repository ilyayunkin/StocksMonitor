#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QHeaderView>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>
#include <QMenuBar>
#include <QTabWidget>

#include <QFile>
#include <QTextStream>
#include <QApplication>
#include <QSettings>

#include "StoryWidget.h"
#include "StocksModelsWidget.h"
#include "PocketWidget.h"

MainWindow::MainWindow(PocketModel &pocketModel,
                       ModelsReferenceList &modelsRefs,
                       AbstractSignalizer &signalizer,
                       QWidget *parent)
    : QMainWindow(parent),
      pocketModel(pocketModel),
      models(modelsRefs)
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
                for(auto &modelsRef : modelsRefs)
                {
                    StocksModelsWidget *w = new StocksModelsWidget(modelsRef, pocketModel);
                    QHBoxLayout *viewLay = new QHBoxLayout;
                    viewLay->setMargin(0);

                    tabWidget->addTab(w, modelsRef.name);
                }
                {
                    PocketWidget *pocketWidget = new PocketWidget(&pocketModel);
                    tabWidget->addTab(pocketWidget, QIcon("://img/portfolio.png"), tr("Portfolio"));
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
                        [&signalizer](){signalizer.changeSound();});
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
#endif
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    QApplication::quit();
}
