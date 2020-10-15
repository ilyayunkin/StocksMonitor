#include "CurrencyConverter.h"

#include <map>
#include <algorithm>

#include <QString>
#include <QStringList>

#include <QInputDialog>

#include <QDebug>

CurrencyConverter::CurrencyConverter(const char *const currencyCode,
                                     StocksInterface * const currencyModel) :
    currencyModel(currencyModel),
    currencyCode(currencyCode)
{
}

CurrencyCountersList CurrencyConverter::convert(const char *const targetCurrency,
                                                const CurrencyCountersList &counters)
{
    assert(targetCurrency);
    assert(strlen(targetCurrency) != 0);
    qDebug() << __PRETTY_FUNCTION__ << targetCurrency;
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
                                 const float value)
{
    assert(targetCurrency);
    assert(strlen(targetCurrency) != 0);
    assert(currency);
    assert(strlen(currency) != 0);

    if(strcmp(currency, targetCurrency) != 0)
    {
        double cource = 0;
        auto getDouble = [&currency, &targetCurrency]
        {
            return QInputDialog::getDouble(0,
                                    QObject::tr("Input cource"),
                                    QObject::tr("Cource of %1 to %2")
                                    .arg(currency)
                                    .arg(targetCurrency),
                                    0,
                                    0,
                                    10000,
                                    8);
        };
        if(currencyModel != nullptr)
        {
            if(currencyCode == targetCurrency)
            {
                Stock stock = currencyModel->getStock(currency);
                if(stock.price != -1)
                    cource = stock.price;
            }else if(currencyCode == currency)
            {
                Stock stock = currencyModel->getStock(targetCurrency);
                if(stock.price != -1)
                    cource = 1 / stock.price;
            }else
            {
                cource = getDouble();
            }
        }else
        {
            cource = getDouble();
        }
        qDebug() << __PRETTY_FUNCTION__ << currency << cource;

        qDebug() << __PRETTY_FUNCTION__ << currency << value * cource;
        return value * cource;
    }else
    {
        qDebug() << __PRETTY_FUNCTION__ << currency;
        return value;
    }
}
