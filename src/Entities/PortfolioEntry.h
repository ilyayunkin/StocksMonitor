#ifndef PORTFOLIOENTRY_H
#define PORTFOLIOENTRY_H

#include <vector>

#include <QString>
#include <QByteArray>

#include "Entities/StocksListHandler.h"

struct PortfolioEntry
{
    QString plugin;
    QString name;
    QByteArray ticker;
    int quantity;
    float price;
    float sellPrice;
    float sum;
    QByteArray currency;
    stocksListHandler handler;
};
typedef std::vector<PortfolioEntry> PortfolioEntryList;

#endif // PORTFOLIOENTRY_H
