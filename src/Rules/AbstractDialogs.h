#ifndef ABSTRACTDIALOGS_H
#define ABSTRACTDIALOGS_H

#include <QByteArray>

class AbstractDialogs
{
public:
    virtual bool askReplaceBuyRequest(const QByteArray &ticker, const float sellPrice) = 0;
    virtual bool askDeleteFromPortfolio(const QByteArray &ticker) = 0;
    virtual bool askAddQuantityToPortfolio(const QByteArray &ticker) = 0;

};

#endif // ABSTRACTDIALOGS_H
