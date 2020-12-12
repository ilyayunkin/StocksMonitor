#ifndef VIEWINTERFACES_H
#define VIEWINTERFACES_H

#include "StocksInterface.h"
#include "BuyRequestInterface.h"

#include <vector>

class AbstractDialogs;

struct ViewInterfacesPair
{
    QString name;
    Url url;
    StocksInterface stocksInterface;
    BuyRequestInterface buyRequestInterface;

    ViewInterfacesPair(const QString name,
                       const Url url,
                       const AbstractDialogs &dialogs,
                       const AbstractBrowser &browser,
                       const Entities &entities,
                       Subscriptions &subscriptions,
                       LoadStocksInteractor &loadStocksInteractor,
                       EditPortfolioInteractor &editPortfolioInteractor,
                       EditBuyRequestInteractor &editBuyRequestInteractor,
                       const stocksListHandler handler) :
        name(std::move(name)),
        url(std::move(url)),
        stocksInterface(entities,
                        subscriptions,
                        loadStocksInteractor,
                        editBuyRequestInteractor,
                        editPortfolioInteractor,
                        dialogs,
                        browser,
                        handler),
        buyRequestInterface(entities,
                            subscriptions,
                            loadStocksInteractor,
                            editPortfolioInteractor,
                            editBuyRequestInteractor,
                            handler)
    {}
};

typedef std::vector<ViewInterfacesPair> ViewInterfacesList;

#endif // VIEWINTERFACES_H
