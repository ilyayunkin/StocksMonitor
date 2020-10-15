#include "StockId.h"

bool operator==(const StockId &lhs, const StockId &rhs)
{
    return lhs.ticker == rhs.ticker;
}
