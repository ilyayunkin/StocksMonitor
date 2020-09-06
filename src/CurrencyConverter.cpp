#include "CurrencyConverter.h"

#include <map>
#include <algorithm>

#include <QString>
#include <QStringList>

#include <QInputDialog>

#include <QDebug>
CurrencyCountersList CurrencyConverter::convert(const QByteArray &targetCurrency,
                                                const CurrencyCountersList &counters)
{
    qDebug() << __PRETTY_FUNCTION__ << targetCurrency;
    std::map<QByteArray, double> currencyCources;
    for(const auto &currency : counters.list)
    {
        if(currency.currency != targetCurrency)
        {
            auto cource = QInputDialog::getDouble(0,
                                                  QObject::tr("Input cource"),
                                                  QObject::tr("Cource of %1 to %2")
                                                  .arg(QString(currency.currency))
                                                  .arg(QString(targetCurrency)),
                                                  0,
                                                  0,
                                                  10000,
                                                  8);
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
