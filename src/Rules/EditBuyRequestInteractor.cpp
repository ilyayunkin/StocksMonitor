#include "EditBuyRequestInteractor.h"

#include "Entities/Entities.h"
#include "Subscriptions.h"
#include "AbstractDialogs.h"
#include "AbstractBuyRequestDatabase.h"

EditBuyRequestInteractor::EditBuyRequestInteractor(Entities &entities,
                                                   Subscriptions &subscriptions)
    : entities(entities)
    , subscriptions(subscriptions)
{
}

void EditBuyRequestInteractor::setDialogs(AbstractDialogs * const dialogs)
{
    this->dialogs = dialogs;
}

void EditBuyRequestInteractor::addDatabase(AbstractBuyRequestDatabase *db)
{
    buyRequestDatabases.push_back(db);
}

void EditBuyRequestInteractor::addLimit(const stocksListHandler handler, const char * const ticker, float referencePrice)
{
    assert(ticker);
    assert(strlen(ticker) != 0);
    auto &stockLimits = entities.pairs[handler].limits;
    const auto &limitsDb = buyRequestDatabases[handler];

    auto it = std::find_if(stockLimits.begin(), stockLimits.end(),
                           [&](const StockLimit &l){return l.ticker == ticker;});

    if(it != stockLimits.end())
    {
        if(dialogs->askReplaceBuyRequest(it->ticker.data(), it->basePrice))
        {
            it->basePrice = referencePrice;
            //                int row = it - stockLimits.begin();

            limitsDb->update(*it);
            subscriptions.updateBuyRequestView(handler);
        }

        return;
    }else
    {
        Stock stock = entities.getStock(handler, ticker);
        StockLimit newEntry{stock, referencePrice};
        stockLimits.push_back(newEntry);
        limitsDb->add(newEntry);
        subscriptions.updateBuyRequestView(handler);
    }
}

bool EditBuyRequestInteractor::setReferencePrice(const stocksListHandler handler, size_t row, float referencePrice)
{
    auto &pair = entities.pairs[handler];
    auto &limits = pair.limits;
    limits[row].basePrice = referencePrice;
    buyRequestDatabases[handler]->update(limits.at(row));
    return true;
}
