#include "Dialogs.h"

#include <QMessageBox>
#include <QObject>

Dialogs::Dialogs(QWidget * const w) :
    w(w)
{
}

bool Dialogs::askReplaceBuyRequest(const QByteArray &ticker, const float sellPrice)
{
    return
        QMessageBox::question(w,QObject::tr("Replace?"),
        QObject::tr("There already is %1:\n %1 %2")
        .arg(QString(ticker))
        .arg(sellPrice))
            == QMessageBox::Yes;
}

bool Dialogs::askDeleteFromPortfolio(const QByteArray &ticker)
{
    return QMessageBox::question(w,
                                 QObject::tr("Delete?"),
                                 QObject::tr("You've selected 0 items:\n %1")
                                 .arg(QString(ticker)))
            == QMessageBox::Yes;
}

bool Dialogs::askAddQuantityToPortfolio(const QByteArray &ticker)
{
    return QMessageBox::question(w,
                                 QObject::tr("Add?"),
                                 QObject::tr("There already is %1:\n %1")
                                 .arg(QString(ticker)))
           == QMessageBox::Yes;
}
