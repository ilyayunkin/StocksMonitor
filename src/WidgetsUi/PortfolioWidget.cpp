#include "PortfolioWidget.h"
#include "ui_PortfolioWidget.h"

#include <QClipboard>

#include <QInputDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include <assert.h>

#include "Application/Application.h"
#include "ViewModels/PortfolioModel.h"

PortfolioWidget::PortfolioWidget(PortfolioModel *model,
                                 Application &application,
                                 QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortfolioWidget),
    model(*model),
    rules(application)
{
    ui->setupUi(this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(PortfolioModel::NAME, Qt::AscendingOrder);
    {
        auto priceUpdater = [this, &application]()
        {ui->sumTextEdit->setText(application.getPortfolioPrice());};
        connect(model, &PortfolioModel::updated,
                priceUpdater);
    }
}

PortfolioWidget::~PortfolioWidget()
{
    delete ui;
}

void PortfolioWidget::on_clipBoardButton_clicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(ui->sumTextEdit->text());
}

void PortfolioWidget::on_convertButton_clicked()
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
