#ifndef ABSTRACTSTOCKSMODEL_H
#define ABSTRACTSTOCKSMODEL_H

#include <QList>
#include <QString>

struct Stock
{
    int rowNum;
    QString name;
    QString ticker;
    float price;
    float derivation;
    float derivationWeek;
    float derivationMonth;
    float derivationYear;
};
typedef QList<Stock> StocksList;

class AbstractStocksModel
{
public:
    virtual void setStocks(StocksList &&stocks) = 0;
    virtual ~AbstractStocksModel(){};
};

#endif // ABSTRACTSTOCKSMODEL_H
