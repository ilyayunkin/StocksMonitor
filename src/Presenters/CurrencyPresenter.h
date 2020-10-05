#ifndef CURRENCYPRESENTER_H
#define CURRENCYPRESENTER_H

#include "Rules/CurrencyCounter.h"

#include <QString>

class CurrencyPresenter
{
public:
    static QString toText(const CurrencyCountersList &counters)
    {
        QString text;
        for(const CurrencyCounter &c : counters.list)
        {
            text+= QString::number(c.sum) + " " + c.currency + " ";
        }
        return text;
    }
};

#endif // CURRENCYPRESENTER_H
