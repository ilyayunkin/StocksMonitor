#ifndef CURRENCYCONVERTER_H
#define CURRENCYCONVERTER_H

#include "CurrencyCounter.h"

class CurrencyConverter
{
public:
    static CurrencyCountersList convert(const QByteArray &targetCurrency,
                                        const CurrencyCountersList &counters);
};

#endif // CURRENCYCONVERTER_H
