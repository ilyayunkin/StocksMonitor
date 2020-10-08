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
};

#endif // DIALOGS_H
