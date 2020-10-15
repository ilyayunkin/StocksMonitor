#ifndef STOCKID_H
#define STOCKID_H

#include <string>
#include <vector>

#include <QString>

struct StockId
{
    std::string ticker;
    QString name;
    StockId(const std::string &ticker, const QString &name)
        : ticker(ticker)
        , name(name)
    {}
};
bool operator==(const StockId &lhs, const StockId &rhs);

typedef std::vector<StockId> StockIdList;

#endif // STOCKID_H
