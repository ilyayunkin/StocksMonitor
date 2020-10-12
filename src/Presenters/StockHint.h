#ifndef STOCKHINT_H
#define STOCKHINT_H

#include "StocksList.h"

class StockHint
{
public:
    static QString getHint(const Stock &stock)
    {
        return QString("%1 | %2 | %3 | %4 | %5 | %6 | %7")
                .arg(stock.name)
                .arg(QString(stock.ticker.data()))
                .arg(stock.price)
                .arg(stock.derivation)
                .arg(stock.derivationWeek)
                .arg(stock.derivationMonth)
                .arg(stock.derivationYear);
    }
};

#endif // STOCKHINT_H
