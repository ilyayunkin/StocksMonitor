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
};
typedef std::vector<Stock> StocksList;

#endif // STOCKSLIST_H
