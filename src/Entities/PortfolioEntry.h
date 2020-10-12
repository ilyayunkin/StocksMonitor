#ifndef PORTFOLIOENTRY_H
#define PORTFOLIOENTRY_H

#include <vector>
#include <string>

#include <QString>

#include "Entities/StocksListHandler.h"

struct PortfolioEntry
{
    QString plugin;
    QString name;
    std::string ticker;
    int quantity;
    float price;
    float sellPrice;
    float sum;
    std::string currency;
    stocksListHandler handler;
};
typedef std::vector<PortfolioEntry> PortfolioEntryList;

#endif // PORTFOLIOENTRY_H
