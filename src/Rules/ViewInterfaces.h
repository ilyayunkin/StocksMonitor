#ifndef VIEWINTERFACES_H
#define VIEWINTERFACES_H

#include "Rules/StocksInterface.h"
#include "Rules/BuyRequestInterface.h"

#include <vector>

struct ViewInterfacesPair
{
    QString name;
    StocksInterface stocksInterfaces;
    BuyRequestInterface buyRequestInterfaces;

    ViewInterfacesPair(const QString &name,
                       const Entities &entities,
                       Subscriptions &subscriptions,
                       LoadStocksInteractor &loadStocksInteractor,
                       EditPortfolioInteractor &editPortfolioInteractor,
                       EditBuyRequestInteractor &editBuyRequestInteractor,
                       const stocksListHandler handler) :
        name(name),
        stocksInterfaces(entities,
                         subscriptions,
                         loadStocksInteractor,
                         editBuyRequestInteractor,
                         editPortfolioInteractor,
                         handler),
        buyRequestInterfaces(entities,
                             subscriptions,
                             loadStocksInteractor,
                             editPortfolioInteractor,
                             editBuyRequestInteractor,
                             handler)
    {}
};

typedef std::vector<ViewInterfacesPair> ViewInterfacesList;

#endif // VIEWINTERFACES_H
