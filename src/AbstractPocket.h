#ifndef ABSTRACTPOCKET_H
#define ABSTRACTPOCKET_H

#include <QString>
#include <QByteArray>

class AbstractPocket
{
public:
    virtual void addStock(QString plugin, QByteArray ticker, int quantity) = 0;
    virtual QString sum() const = 0;
    virtual ~AbstractPocket() = default;
};

#endif // ABSTRACTPOCKET_H
