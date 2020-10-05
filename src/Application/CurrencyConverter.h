#ifndef CURRENCYCONVERTER_H
#define CURRENCYCONVERTER_H

#include "Rules/AbstractCurrencyConverter.h"
#include "Rules/CurrencyCounter.h"

class AbstractStocksModel;

class CurrencyConverter final : public AbstractCurrencyConverter
{
    AbstractStocksModel * const currencyModel;
public:
    CurrencyConverter(AbstractStocksModel *const currencyModel = nullptr);
    CurrencyCountersList convert(const QByteArray &targetCurrency,
                                 const CurrencyCountersList &counters) override;
};

#endif // CURRENCYCONVERTER_H
