#include "PocketWidget.h"
#include "ui_PocketWidget.h"

#include <QTimer>

PocketWidget::PocketWidget(PocketModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PocketWidget)
{
    ui->setupUi(this);
    ui->tableView->setModel(model);
    {
        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout,
                [this, model](){ui->label->setText(model->sum());});
        t->start(5000);
    }
}

PocketWidget::~PocketWidget()
{
    delete ui;
}
