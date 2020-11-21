#ifndef LOADSTOCKSINTERACTOR_H
#define LOADSTOCKSINTERACTOR_H

#include "AbstractStocksReceiver.h"

class Entities;
class AbstractNotifier;
class Subscriptions;

class LoadStocksInteractor final : public AbstractStocksReceiver
{
public:
    LoadStocksInteractor(Entities &entities,
                         Subscriptions &subscriptions);
    void setStocks(const stocksListHandler handler,
                   StocksList &&stocks,
                   const char *const time) override;
    void setNotifier(AbstractNotifier * const notifier);
private:
    Entities &entities;
    Subscriptions &subscriptions;
    AbstractNotifier *notifier = nullptr;

    void updateBuyRequestsFromStocks(const stocksListHandler handler);
    void updatePortfolioPricesFromStocks(const stocksListHandler handler);
    void signalizePortfolio(const StockName &name, const float price);
    void signalizeLimit(const StockName &name, const float price);
};

#endif // LOADSTOCKSINTERACTOR_H
