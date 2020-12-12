#ifndef EDITBUYREQUESTINTERACTOR_H
#define EDITBUYREQUESTINTERACTOR_H

#include "Entities/StocksListHandler.h"
#include <vector>
class Entities;
class Subscriptions;
class AbstractDialogs;
class AbstractBuyRequestDatabase;

typedef std::vector<AbstractBuyRequestDatabase *> BuyRequestDatabasesList;

class EditBuyRequestInteractor
{
public:
    EditBuyRequestInteractor(Entities &entities,
                             Subscriptions &subscriptions,
                             const AbstractDialogs &dialogs);
    void addDatabase(AbstractBuyRequestDatabase *db);

    void addLimit(const stocksListHandler handler,
                  const char *const ticker,
                  float referencePrice);
    bool setReferencePrice(const stocksListHandler handler,
                           size_t row,
                           float referencePrice);
    void remove(const stocksListHandler handler,
                const char *const ticker);
private:
    Entities &entities;
    Subscriptions &subscriptions;
    const AbstractDialogs &dialogs;
    BuyRequestDatabasesList buyRequestDatabases;
};

#endif // EDITBUYREQUESTINTERACTOR_H
