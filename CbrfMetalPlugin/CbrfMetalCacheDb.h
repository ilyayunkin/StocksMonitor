#ifndef CBRFMETALCACHEDB_H
#define CBRFMETALCACHEDB_H

#include "StocksList.h"
#include <QDate>
#include <optional>
class CbrfMetalCacheDb
{
public:
    struct CacheElement{
        QDate date;
        QByteArray code;
        float priceBuy;
        float priceSell;
    };
    typedef std::vector<CacheElement> CacheElementsList;
    struct OutElement{
        QByteArray code;
        float priceBuy;
        float derivation;
        float derivationWeek;
        float derivationMonth;
        float derivationYear;
    };
    typedef std::vector<OutElement> OutElementList;
    struct OutLine{
        QDate date;
        OutElementList list;
    };
    CbrfMetalCacheDb();
    bool empty();
    std::size_t size();
    void pushNew(const CacheElement &newLine);
    const CacheElement * getLine(const QDate &date, const QByteArray &code);
    OutLine getCached();
private:
    CacheElementsList list;
    OutLine cached;
};

#endif // CBRFMETALCACHEDB_H
