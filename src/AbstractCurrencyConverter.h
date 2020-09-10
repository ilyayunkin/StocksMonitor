#ifndef ABSTRACTCURRENCYCONVERTER_H
#define ABSTRACTCURRENCYCONVERTER_H

#include <QByteArray>
#include "CurrencyCounter.h"

class AbstractCurrencyConverter
{
public:
    virtual CurrencyCountersList convert(const QByteArray &targetCurrency,
                                 const CurrencyCountersList &counters) = 0;
};

#endif // ABSTRACTCURRENCYCONVERTER_H
