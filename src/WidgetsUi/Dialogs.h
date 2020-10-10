#ifndef DIALOGS_H
#define DIALOGS_H

#include "Rules/AbstractDialogs.h"

class QWidget;
class Dialogs : public AbstractDialogs
{
    QWidget *const w = nullptr;
public:
    Dialogs(QWidget *const w = nullptr);
    bool askReplaceBuyRequest(const QByteArray &ticker, const float sellPrice) override;
    bool askDeleteFromPortfolio(const QByteArray &ticker) override;
    bool askAddQuantityToPortfolio(const QByteArray &ticker) override;
    bool askBuyRequestDoubleDeletion(const QByteArray &ticker1, const float basePrice1, const QByteArray &ticker2, const float basePrice2);
};

#endif // DIALOGS_H
