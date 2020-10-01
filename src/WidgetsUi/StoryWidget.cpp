#include "StoryWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>
#include <QMessageBox>

#include "logger.h"

StoryWidget::StoryWidget(QWidget *parent) :
    QDialog(parent)
{
    qDebug() << __PRETTY_FUNCTION__;
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    {
        QTextEdit *te = new QTextEdit;
        mainLayout->addWidget(te);

        te->setReadOnly(true);
        te->setText(Logger::instance().getLog());
    }
    {
        QHBoxLayout *hlay = new QHBoxLayout;
        mainLayout->addLayout(hlay);

        hlay->setMargin(0);
        {
            QPushButton *okButton = new QPushButton(tr("OK"));
            hlay->addWidget(okButton);
            connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        }
    }
}

StoryWidget::~StoryWidget()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void StoryWidget::showStory()
{
    try
    {
        StoryWidget w;
        w.exec();
    } catch (std::runtime_error &e)
    {
        QMessageBox::critical(0, tr("Error"), e.what());
    } catch(...)
    {
        QMessageBox::critical(0, tr("Error"), tr("Unknown error"));
    }
}
