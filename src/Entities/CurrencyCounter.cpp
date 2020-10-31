#include "CurrencyCounter.h"

#include <algorithm>

void CurrencyCountersList::add(const char * const currency, const double val)
{
    auto it = std::find_if(list.begin(), list.end(), [currency](const CurrencyCounter &c){return currency == c.currency;});
    if(it != list.end())
    {
        it->sum+= val;
    }else
    {
        list.push_back(CurrencyCounter(currency, val));
    }
}
