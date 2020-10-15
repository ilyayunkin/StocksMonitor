#ifndef ABSTRACTDIALOGS_H
#define ABSTRACTDIALOGS_H

#include <QString>

class AbstractDialogs
{
public:
    virtual bool askReplaceBuyRequest(const char *const ticker
                                      , const float sellPrice)const = 0;
    virtual bool askDeleteFromPortfolio(const char *const ticker)const = 0;
    virtual bool askAddQuantityToPortfolio(const char *const ticker)const = 0;
    virtual bool askReplaceItemFromStatistics(const char *const ticker)const = 0;
    virtual bool askReplaceGroupStatistics(QString const &name)const = 0;
    virtual bool askReplaceCategoryStatistics(QString const &name)const = 0;
};

#endif // ABSTRACTDIALOGS_H
