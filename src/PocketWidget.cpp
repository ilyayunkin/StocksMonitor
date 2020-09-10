#include "PocketWidget.h"
#include "ui_PocketWidget.h"

#include <QTimer>
#include <QClipboard>

#include <QInputDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include <algorithm>
#include <set>
#include <assert.h>

#include "Presenters/CurrencyPresenter.h"
#include "CurrencyConverter.h"

PocketWidget::PocketWidget(PocketModel *model,
                           AbstractCurrencyConverter &currencyConverter,
                           QWidget *parent) :
    QWidget(parent),
    model(*model),
    currencyConverter(currencyConverter),
    ui(new Ui::PocketWidget)
{
    ui->setupUi(this);
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(PocketModel::NAME, Qt::AscendingOrder);
    {
        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout,
                [this, model](){ui->sumTextEdit->setText(CurrencyPresenter::toText(model->sum()));});
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
    auto counters = model.sum();

    assert(!counters.list.empty());

    std::set<QByteArray> currencySet;
    for(const auto &c : counters.list)
    {
        currencySet.insert(c.currency);
    }
    QStringList items;
    std::transform(currencySet.begin(), currencySet.end(), std::back_inserter(items),
                   [](const QString &str){return str;});
    bool ok;
    QByteArray targetCurrency =
            QInputDialog::getItem(0,
                                  QObject::tr("Select currency"),
                                  QObject::tr("Currency"),
                                  items, 0, false, &ok).toLatin1();
    if(ok)
    {
        auto convertedCounters = CurrencyPresenter::toText(currencyConverter.convert(targetCurrency, counters));
        QMessageBox::information(this,
                                 tr("Portfolio in %1").arg(QString(targetCurrency)),
                                 convertedCounters);
    }
}
