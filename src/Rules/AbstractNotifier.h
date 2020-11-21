#ifndef ABSTRACTNOTIFIER_H
#define ABSTRACTNOTIFIER_H

#include "StringTypes.h"

class AbstractNotifier
{
public:
    virtual ~AbstractNotifier() = default;
    virtual void signalizePortfolio(const StockName &name, const float price) = 0;
    virtual void signalizeLimit(const StockName &name, const float price) = 0;
};

#endif // ABSTRACTNOTIFIER_H
