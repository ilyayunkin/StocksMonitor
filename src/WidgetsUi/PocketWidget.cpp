#include "PocketWidget.h"
#include "ui_PocketWidget.h"

#include <QTimer>
#include <QClipboard>

#include <QInputDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include <assert.h>

#include "Application/Application.h"
#include "PortfolioModel.h"

PocketWidget::PocketWidget(PortfolioModel *model,
                           Application &application,
                           QWidget *parent) :
    QWidget(parent),
    model(*model),
    rules(application),
    ui(new Ui::PocketWidget)
{
    ui->setupUi(this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(PortfolioModel::NAME, Qt::AscendingOrder);
    {
        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout,
                [this, &application](){ui->sumTextEdit->setText(application.getPortfolioPrice());});
        t->start(5000);
    }
}

PocketWidget::~PocketWidget()
{
    delete ui;
}

void PocketWidget::on_clipBoardButton_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->sumTextEdit->text());
}

void PocketWidget::on_convertButton_clicked()
{
    QStringList items = rules.getAvailibleCurrencies();

    bool ok;
    QByteArray targetCurrency =
            QInputDialog::getItem(0,
                                  QObject::tr("Select currency"),
                                  QObject::tr("Currency"),
                                  items, 0, false, &ok).toLatin1();
    if(ok)
    {
        auto convertedCounters = rules.getPortfolioPrice(targetCurrency.data());
        QMessageBox::information(this,
                                 tr("Portfolio in %1").arg(QString(targetCurrency)),
                                 convertedCounters);
    }
}
