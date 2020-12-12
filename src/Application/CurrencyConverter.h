#ifndef CURRENCYCONVERTER_H
#define CURRENCYCONVERTER_H

#include "Rules/AbstractCurrencyConverter.h"
#include "Entities/CurrencyCounter.h"
#include "AbstractCurrencyCourseSource.h"

class AbstractDialogs;
class AbstractCurrencyCourseSource;

class CurrencyConverter final : public AbstractCurrencyConverter
{
public:
    explicit CurrencyConverter(const char *const currencyCode,
                               const AbstractDialogs &dialogs,
                               AbstractCurrencyCourseSource *const currencyModel = nullptr);
    CurrencyCountersList convert(const char *const targetCurrency,
                                 const CurrencyCountersList &counters)const override;
    float convert(const char *const targetCurrency,
                  const char *const currency,
                  const float value)const override;
    void setCurrencyModel(AbstractCurrencyCourseSource *const currencyModel);
private:
    AbstractCurrencyCourseSource * currencyModel;
    const QByteArray currencyCode;
    const AbstractDialogs &dialogs;
};

#endif // CURRENCYCONVERTER_H
