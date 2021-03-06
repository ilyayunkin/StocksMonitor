#include "CurrencyConverter.h"

#include <map>
#include <algorithm>

#include <QString>
#include <QStringList>

#include "Rules/AbstractDialogs.h"
#include "StocksList.h"

CurrencyConverter::CurrencyConverter(const char *const currencyCode,
                                     const AbstractDialogs &dialogs,
                                     AbstractCurrencyCourseSource * const currencyModel)
    : currencyModel(currencyModel)
    , currencyCode(currencyCode)
    , dialogs(dialogs)
{
}

CurrencyCountersList CurrencyConverter::convert(const char *const targetCurrency,
                                                const CurrencyCountersList &counters) const
{
    assert(targetCurrency);
    assert(strlen(targetCurrency) != 0);

    CurrencyCountersList convertedCounters;
    for(const auto &currency : counters.list)
    {
        convertedCounters.add(targetCurrency,
                              convert(targetCurrency,
                                      currency.currency.data(),
                                      currency.sum));
    }

    return convertedCounters;
}

float CurrencyConverter::convert(const char * const targetCurrency,
                                 const char * const currency,
                                 const float value) const
{
    assert(targetCurrency);
    assert(strlen(targetCurrency) != 0);
    assert(currency);
    assert(strlen(currency) != 0);

    if(strcmp(currency, targetCurrency) != 0)
    {
        double course = [&]
        {
            if(currencyModel != nullptr)
            {
                if(currencyCode == targetCurrency)
                {
                    auto price = currencyModel->getCurrencyCourse(currency);
                    if(price != Stock::defaultPrice)
                        return price;
                }else if(currencyCode == currency)
                {
                    auto price = currencyModel->getCurrencyCourse(targetCurrency);
                    if(price != Stock::defaultPrice)
                        return 1 / price;
                }
                return dialogs.askCurrencyCourse(currency, targetCurrency);
            }
            return dialogs.askCurrencyCourse(currency, targetCurrency);
        }();

        return value * course;
    }else
    {
        return value;
    }
}

void CurrencyConverter::setCurrencyModel(AbstractCurrencyCourseSource * const currencyModel)
{
    this->currencyModel = currencyModel;
}
