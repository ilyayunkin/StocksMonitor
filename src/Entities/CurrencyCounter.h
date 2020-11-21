#ifndef CURRENCYCOUNTER_H
#define CURRENCYCOUNTER_H

#include <vector>
#include "StringTypes.h"

struct CurrencyCounter
{
    CurrencyCode currency;
    double sum;
    explicit CurrencyCounter(const char *const currency) : currency(currency), sum(0){}
    CurrencyCounter(const char *const currency, const double sum) : currency(currency), sum(sum){}
};

struct CurrencyCountersList
{
    std::vector<CurrencyCounter> list;
    void add(const char *const currency, const double val);
};

#endif // CURRENCYCOUNTER_H
