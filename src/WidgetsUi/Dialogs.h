#ifndef DIALOGS_H
#define DIALOGS_H

#include "Rules/AbstractDialogs.h"

class QWidget;
class Dialogs : public AbstractDialogs
{
    QWidget *const w = nullptr;
public:
    Dialogs(QWidget *const w = nullptr);
    bool askReplaceBuyRequest(const char *const ticker, const float sellPrice) override;
    bool askDeleteFromPortfolio(const char *const ticker) override;
    bool askAddQuantityToPortfolio(const char *const ticker) override;
    bool askBuyRequestDoubleDeletion(const char *const ticker1, const float basePrice1, const char *const ticker2, const float basePrice2);
};

#endif // DIALOGS_H
