#ifndef ABSTRACTSTOCKSMODEL_H
#define ABSTRACTSTOCKSMODEL_H

#include <QList>
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

class AbstractStocksModel
{
public:
    virtual void setStocks(StocksList &&stocks) = 0;
    virtual size_t size() = 0;
    virtual Stock getStock(const QByteArray &ticker) = 0;
    virtual Stock getStock(const size_t i) = 0;
    virtual float getStockPrice(const QByteArray &ticker) = 0;
    virtual ~AbstractStocksModel(){};
};

#endif // ABSTRACTSTOCKSMODEL_H
