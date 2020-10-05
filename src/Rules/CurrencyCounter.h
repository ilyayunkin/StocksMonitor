#ifndef CURRENCYCOUNTER_H
#define CURRENCYCOUNTER_H

#include <QByteArray>
#include <vector>

struct CurrencyCounter
{
    QByteArray currency;
    double sum;
    CurrencyCounter(const QByteArray &currency) : currency(currency), sum(0){}
    CurrencyCounter(const QByteArray &currency, const double sum) : currency(currency), sum(sum){}
};

struct CurrencyCountersList
{
    std::vector<CurrencyCounter> list;
    void add(const QByteArray &currency, const double val)
    {
        auto it = std::find_if(list.begin(), list.end(), [currency](CurrencyCounter &c){return currency == c.currency;});
        if(it != list.end())
        {
            it->sum+= val;
        }else
        {
            list.push_back(CurrencyCounter(currency, val));
        }
    }
};

#endif // CURRENCYCOUNTER_H
