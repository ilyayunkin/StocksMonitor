#ifndef CURRENCYCONVERTER_H
#define CURRENCYCONVERTER_H

#include "Rules/AbstractCurrencyConverter.h"
#include "Entities/CurrencyCounter.h"
#include "StocksInterface.h"

class CurrencyConverter final : public AbstractCurrencyConverter
{
public:
    explicit CurrencyConverter(const char *const currencyCode,
                      StocksInterface *const currencyModel = nullptr);
    CurrencyCountersList convert(const char *const targetCurrency,
                                 const CurrencyCountersList &counters) override;
    float convert(const char *const targetCurrency,
                              const char *const currency,
                              const float value) override;
private:
    StocksInterface * const currencyModel;
    const QByteArray currencyCode;
};

#endif // CURRENCYCONVERTER_H
