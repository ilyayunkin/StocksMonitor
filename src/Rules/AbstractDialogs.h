#ifndef ABSTRACTDIALOGS_H
#define ABSTRACTDIALOGS_H

class AbstractDialogs
{
public:
    virtual bool askReplaceBuyRequest(const char *const ticker, const float sellPrice) = 0;
    virtual bool askDeleteFromPortfolio(const char *const ticker) = 0;
    virtual bool askAddQuantityToPortfolio(const char *const ticker) = 0;

};

#endif // ABSTRACTDIALOGS_H
