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
    CurrencyConverter(const QByteArray &currencyCode,
                      StocksInterface *const currencyModel = nullptr);
    CurrencyCountersList convert(const QByteArray &targetCurrency,
                                 const CurrencyCountersList &counters) override;
};

#endif // CURRENCYCONVERTER_H
