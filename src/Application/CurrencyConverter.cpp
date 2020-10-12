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
    qDebug() << __PRETTY_FUNCTION__ << targetCurrency;
    std::map<QByteArray, double> currencyCources;
    for(const auto &currency : counters.list)
    {
        if(currency.currency != targetCurrency)
        {
            double cource = 0;
            auto getDouble = [&currency, &targetCurrency]
            {
                return QInputDialog::getDouble(0,
                                        QObject::tr("Input cource"),
                                        QObject::tr("Cource of %1 to %2")
                                        .arg(currency.currency.data())
                                        .arg(targetCurrency),
                                        0,
                                        0,
                                        10000,
                                        8);
            };
            if(currencyModel != nullptr)
            {
                const QByteArray baseCurrency = currencyCode;
                if(baseCurrency == targetCurrency)
                {
                    Stock stock = currencyModel->getStock(currency.currency.data());
                    if(stock.price != -1)
                        cource = stock.price;
                }else if(baseCurrency == currency.currency.data())
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
            qDebug() << __PRETTY_FUNCTION__ << currency.currency.data() << cource;
            currencyCources[currency.currency.data()] = cource;
        }
    }

    CurrencyCountersList convertedCounters;
    for(const auto &counter : counters.list)
    {
        if(counter.currency == targetCurrency)
        {
            qDebug() << __PRETTY_FUNCTION__ << counter.currency.data();
            convertedCounters.add(targetCurrency, counter.sum);
        }else
        {
            qDebug() << __PRETTY_FUNCTION__ << counter.currency.data() << counter.sum * currencyCources[counter.currency.data()];
            convertedCounters.add(targetCurrency, counter.sum * currencyCources[counter.currency.data()]);
        }
    }

    return convertedCounters;
}
