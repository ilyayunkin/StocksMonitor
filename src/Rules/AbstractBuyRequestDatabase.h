#ifndef ABSTRACTBUYREQUESTDATABASE_H
#define ABSTRACTBUYREQUESTDATABASE_H

#include "Entities/StockLimit.h"

class AbstractBuyRequestDatabase
{
public:
    virtual ~AbstractBuyRequestDatabase() = default;
    virtual void add(const StockLimit &stockLimit) = 0;
    virtual void update(const StockLimit &stockLimit) = 0;
    virtual StockLimitsList getAll() = 0;
};

#endif // ABSTRACTBUYREQUESTDATABASE_H
