#ifndef STOCKSLIST_H
#define STOCKSLIST_H

#include <vector>
#include "StringTypes.h"

struct Stock
{
    static constexpr float defaultPrice = -1;
    static constexpr float defaultDerivation = -1;
    StockName name;
    Ticker ticker;
    Url url;
    float price;
    float derivation;
    float derivationWeek;
    float derivationMonth;
    float derivationYear;

    Stock() :
        price(defaultPrice),
        derivation(defaultDerivation),
        derivationWeek(defaultDerivation),
        derivationMonth(defaultDerivation),
        derivationYear(defaultDerivation)
    {}
    Stock(StockName name,
          Ticker ticker,
          Url url,
          float price,
          float derivation = defaultDerivation,
          float derivationWeek = defaultDerivation,
          float derivationMonth = defaultDerivation,
          float derivationYear = defaultDerivation)
        : name(std::move(name))
        , ticker(std::move(ticker))
        , url(std::move(url))
        , price(price)
        , derivation(derivation)
        , derivationWeek(derivationWeek)
        , derivationMonth(derivationMonth)
        , derivationYear(derivationYear)
    {}
};
typedef std::vector<Stock> StocksList;

#endif // STOCKSLIST_H
