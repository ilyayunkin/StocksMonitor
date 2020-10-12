#ifndef CURRENCYCONVERTER_H
#define CURRENCYCONVERTER_H

#include "Rules/AbstractCurrencyConverter.h"
#include "Rules/CurrencyCounter.h"
#include "Rules/StocksInterface.h"

class CurrencyConverter final : public AbstractCurrencyConverter
{
    StocksInterface * const currencyModel;
    const QByteArray currencyCode;
public:
    explicit CurrencyConverter(const char *const currencyCode,
                      StocksInterface *const currencyModel = nullptr);
    CurrencyCountersList convert(const char *const targetCurrency,
                                 const CurrencyCountersList &counters) override;
};

#endif // CURRENCYCONVERTER_H
