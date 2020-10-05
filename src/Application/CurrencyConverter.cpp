#include "CurrencyConverter.h"

#include <map>
#include <algorithm>

#include <QString>
#include <QStringList>

#include <QInputDialog>

#include <QDebug>

#include "Rules/AbstractStocksModel.h"

CurrencyConverter::CurrencyConverter(AbstractStocksModel * const currencyModel) :
    currencyModel(currencyModel)
{
}

CurrencyCountersList CurrencyConverter::convert(const QByteArray &targetCurrency,
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
                                        .arg(QString(currency.currency))
                                        .arg(QString(targetCurrency)),
                                        0,
                                        0,
                                        10000,
                                        8);
            };
            if(currencyModel != nullptr)
            {
                const QByteArray baseCurrency = currencyModel->currencyCode();
                if(targetCurrency == baseCurrency)
                {
                    Stock stock = currencyModel->getStock(currency.currency);
                    if(stock.price != -1)
                        cource = stock.price;
                }else if(currency.currency == baseCurrency)
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
            qDebug() << __PRETTY_FUNCTION__ << currency.currency << cource;
            currencyCources[currency.currency] = cource;
        }
    }

    CurrencyCountersList convertedCounters;
    for(const auto &counter : counters.list)
    {
        if(counter.currency == targetCurrency)
        {
            qDebug() << __PRETTY_FUNCTION__ << counter.currency;
            convertedCounters.add(targetCurrency, counter.sum);
        }else
        {
            qDebug() << __PRETTY_FUNCTION__ << counter.currency << counter.sum * currencyCources[counter.currency];
            convertedCounters.add(targetCurrency, counter.sum * currencyCources[counter.currency]);
        }
    }

    return convertedCounters;
}
