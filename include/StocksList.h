#ifndef STOCKSLIST_H
#define STOCKSLIST_H

#include <QString>

#include <vector>

struct Stock
{
    int rowNum;
    QString name;
    QByteArray ticker;
    float price;
    float derivation;
    float derivationWeek;
    float derivationMonth;
    float derivationYear;

    Stock() :
        rowNum(-1),
        price(-1),
        derivation(-1),
        derivationWeek(-1),
        derivationMonth(-1),
        derivationYear(-1)
    {}
};
typedef std::vector<Stock> StocksList;

#endif // STOCKSLIST_H
