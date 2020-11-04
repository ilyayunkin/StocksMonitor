#ifndef STOCKSLIST_H
#define STOCKSLIST_H

#include <QString>

#include <vector>
#include <string>

struct Stock
{
    QString name;
    std::string ticker;
    std::string url;
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
    Stock(QString name,
          std::string ticker,
          std::string url,
          float price,
          float derivation = -1,
          float derivationWeek = -1,
          float derivationMonth = -1,
          float derivationYear = -1)
        : name(name)
        , ticker(ticker)
        , url(url)
        , price(price)
        , derivation(derivation)
        , derivationWeek(derivationWeek)
        , derivationMonth(derivationMonth)
        , derivationYear(derivationYear)
    {}
};
typedef std::vector<Stock> StocksList;

#endif // STOCKSLIST_H
