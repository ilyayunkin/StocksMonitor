#ifndef VIEWINTERFACES_H
#define VIEWINTERFACES_H

#include "Rules/StocksInterface.h"
#include "Rules/BuyRequestInterface.h"

#include <vector>

struct ViewInterfacesPair
{
    QString name;
    StocksInterface stocks;
    BuyRequestInterface buyRequests;

    ViewInterfacesPair(const QString &name, RulesFasade * const rules,
                       const stocksListHandler handler) :
        name(name),
        stocks(rules, handler),
        buyRequests(rules, handler)
    {}
};

typedef std::vector<ViewInterfacesPair> ViewInterfaces;

#endif // VIEWINTERFACES_H
