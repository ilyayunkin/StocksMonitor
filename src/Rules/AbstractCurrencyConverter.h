#ifndef ABSTRACTCURRENCYCONVERTER_H
#define ABSTRACTCURRENCYCONVERTER_H

#include "CurrencyCounter.h"

class AbstractCurrencyConverter
{
public:
    virtual CurrencyCountersList convert(const char *const targetCurrency,
                                 const CurrencyCountersList &counters) = 0;
};

#endif // ABSTRACTCURRENCYCONVERTER_H
