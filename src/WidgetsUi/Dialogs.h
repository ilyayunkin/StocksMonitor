#ifndef DIALOGS_H
#define DIALOGS_H

#include "Rules/AbstractDialogs.h"

class QWidget;
class Dialogs : public AbstractDialogs
{
public:
    Dialogs(QWidget *const w = nullptr);
    bool askReplaceBuyRequest(const char *const ticker
                              , const float sellPrice)const override;
    bool askDeleteFromPortfolio(const char *const ticker)const override;
    bool askAddQuantityToPortfolio(const char *const ticker)const override;
    bool askReplaceItemFromStatistics(const char *const ticker)const override;
    bool askReplaceGroupStatistics(QString const &name)const override;
    bool askReplaceCategoryStatistics(QString const &name)const override;
    bool askBuyRequestDoubleDeletion(const char *const ticker1
                                     , const float basePrice1
                                     , const char *const ticker2
                                     , const float basePrice2)const;
private:
    QWidget *const w = nullptr;
};

#endif // DIALOGS_H
