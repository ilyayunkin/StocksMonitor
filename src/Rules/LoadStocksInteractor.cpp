#include "LoadStocksInteractor.h"
#include "Entities/Entities.h"

#include "AbstractNotifier.h"
#include "AbstractStocksView.h"
#include "Subscriptions.h"

LoadStocksInteractor::LoadStocksInteractor(Entities &entities, Subscriptions &subscriptions)
    : entities(entities)
    , subscriptions(subscriptions)
{

}

void LoadStocksInteractor::setStocks(const stocksListHandler handler, StocksList &&stocks, const char * const time)
{
    auto &pair = entities.pairs[handler];
    if(time != pair.time)
    {
        std::swap(pair.stocks, stocks);
        pair.time = time;

        updateBuyRequestsFromStocks(handler);
        updatePortfolioPricesFromStocks(handler);
        subscriptions.updateStocksView(handler);
    }
}

void LoadStocksInteractor::setNotifier(AbstractNotifier *const notifier)
{
    this->notifier = notifier;
}

void LoadStocksInteractor::updateBuyRequestsFromStocks(const stocksListHandler handler)
{
    auto &stockLimits = entities.pairs[handler].limits;

    int i = 0;
    bool anyUpdates = false;
    for(auto &limit : stockLimits)
    {
        float price = entities.getStockPrice(handler, limit.ticker.data());
        if(price != limit.price)
        {
            anyUpdates = true;
            if((price < limit.basePrice)
                    && ((limit.price >= limit.basePrice)
                        || (limit.price <= 0)))
            {
                signalizeLimit(limit.name, price);
            }
            limit.price = price;
        }
        ++i;
    }
    if(anyUpdates)
    {
        subscriptions.updateBuyRequestView(handler);
    }
}

void LoadStocksInteractor::updatePortfolioPricesFromStocks(const stocksListHandler handler)
{
    const auto currency = entities.pairs[handler].currencyCode;
    bool needUpdate = false;
    for(auto &e : entities.portfolio.portfolio)
    {
        if(e.handler == handler)
        {
            Stock stock = entities.getStock(handler, e.ticker.data());
            if(e.name.isEmpty())
            {
                e.name = stock.name;
                needUpdate = true;
            }
            if(e.price != stock.price)
            {
                auto cross = e.sellPrice > 0
                        && e.price < e.sellPrice
                        && stock.price >= e.sellPrice;
                e.price = stock.price;
                e.derivation = stock.derivation;
                e.derivationWeek = stock.derivationWeek;
                e.derivationYear = stock.derivationYear;
                e.derivationMonth = stock.derivationMonth;
                e.sum = e.price * e.quantity;
                e.currency = currency;

                if(cross)
                {
                    signalizePortfolio(e.name, e.price);
                }
                needUpdate = true;
            }
        }
    }
    if(needUpdate)
    {
        subscriptions.updatePortfolioView();
    }
}

void LoadStocksInteractor::signalizePortfolio(const QString &name, const float price)
{
    assert(notifier);
    notifier->signalizePortfolio(name, price);
}

void LoadStocksInteractor::signalizeLimit(const QString &name, const float price)
{
    assert(notifier);
    notifier->signalizeLimit(name, price);
}

