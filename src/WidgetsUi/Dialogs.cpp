#include "Dialogs.h"

#include <QMessageBox>
#include <QObject>

Dialogs::Dialogs(QWidget * const w) :
    w(w)
{
}

bool Dialogs::askReplaceBuyRequest(const char *const ticker, const float sellPrice)
{
    return
            QMessageBox::question(w,QObject::tr("Replace?"),
                                  QObject::tr("There already is %1:\n %1 %2")
                                  .arg(QString(ticker))
                                  .arg(sellPrice))
            == QMessageBox::Yes;
}

bool Dialogs::askDeleteFromPortfolio(const char *const ticker)
{
    return QMessageBox::question(w,
                                 QObject::tr("Delete?"),
                                 QObject::tr("You've selected 0 items:\n %1")
                                 .arg(QString(ticker)))
            == QMessageBox::Yes;
}

bool Dialogs::askAddQuantityToPortfolio(const char *const ticker)
{
    return QMessageBox::question(w,
                                 QObject::tr("Add?"),
                                 QObject::tr("There already is %1:\n %1")
                                 .arg(QString(ticker)))
            == QMessageBox::Yes;
}

bool Dialogs::askBuyRequestDoubleDeletion(const char *const ticker1, const float basePrice1,
                                          const char *const ticker2, const float basePrice2)
{
    return QMessageBox::question(0, QObject::tr("Delete double?"),
                                 QString("%1 %2\n%3 %4")
                                 .arg(QString(ticker1))
                                 .arg(basePrice1)
                                 .arg(QString(ticker2))
                                 .arg(basePrice2))
            == QMessageBox::Yes;
}
