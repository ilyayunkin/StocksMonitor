#ifndef ABSTRACTNOTIFIER_H
#define ABSTRACTNOTIFIER_H

#include <QString>

class AbstractNotifier
{
public:
    virtual ~AbstractNotifier() = default;
    virtual void signalizePortfolio(const QString &name, const float price) = 0;
    virtual void signalizeLimit(const QString &name, const float price) = 0;
};

#endif // ABSTRACTNOTIFIER_H
