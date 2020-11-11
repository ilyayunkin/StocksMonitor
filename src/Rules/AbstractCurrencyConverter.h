#ifndef ABSTRACTCURRENCYCONVERTER_H
#define ABSTRACTCURRENCYCONVERTER_H

#include "Entities/CurrencyCounter.h"

class AbstractCurrencyConverter
{
public:
    virtual CurrencyCountersList convert(const char *const targetCurrency,
                                         const CurrencyCountersList &counters)const = 0;
    virtual float convert(const char *const targetCurrency,
                          const char *const currency,
                          const float value)const = 0;
};

#endif // ABSTRACTCURRENCYCONVERTER_H
