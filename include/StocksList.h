#ifndef STOCKSLIST_H
#define STOCKSLIST_H

#include <vector>
#include "StringTypes.h"

struct Stock
{
    StockName name;
    Ticker ticker;
    Url url;
    float price;
    float derivation;
    float derivationWeek;
    float derivationMonth;
    float derivationYear;

    Stock() :
        price(-1),
        derivation(-1),
        derivationWeek(-1),
        derivationMonth(-1),
        derivationYear(-1)
    {}
    Stock(StockName name,
          Ticker ticker,
          Url url,
          float price,
          float derivation = -1,
          float derivationWeek = -1,
          float derivationMonth = -1,
          float derivationYear = -1)
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
