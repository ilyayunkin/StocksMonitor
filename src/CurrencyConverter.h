#ifndef CURRENCYCONVERTER_H
#define CURRENCYCONVERTER_H

#include "AbstractCurrencyConverter.h"

class StocksModel;

class CurrencyConverter : public AbstractCurrencyConverter
{
    StocksModel * const currencyModel;
public:
    CurrencyConverter(StocksModel *const currencyModel = nullptr);
    CurrencyCountersList convert(const QByteArray &targetCurrency,
                                 const CurrencyCountersList &counters) override;
};

#endif // CURRENCYCONVERTER_H
