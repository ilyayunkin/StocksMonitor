#ifndef EDITPORTFOLIOINTERACTOR_H
#define EDITPORTFOLIOINTERACTOR_H

#include <QString>
#include "Entities/StocksListHandler.h"

class Entities;
class AbstractDialogs;
class Subscriptions;
class AbstractPortfolioDatabase;
class AbstractCurrencyConverter;

class EditPortfolioInteractor
{
public:
    EditPortfolioInteractor(Entities &entities,
                            Subscriptions &subscriptions);
    void setDialogs(AbstractDialogs *const dialogs);
    void setPortfolioDatabase(AbstractPortfolioDatabase *const portfolioDb);
    void setConverter(AbstractCurrencyConverter *const converter);

    bool setPortfolioEntryQuantity(size_t row, int quantity);
    bool setPortfolioEntryReferencePrice(size_t row, float referencePrice);
    QString getPortfolioPrice(const char *const currency);
    QString getPortfolioPrice();
    void addToPortfolio(const stocksListHandler handler,
                        const char *const ticker, const int quantity);
    void deletePortfolioEntry(size_t row);
private:
    Entities &entities;
    AbstractDialogs *dialogs = nullptr;
    Subscriptions &subscriptions;
    AbstractPortfolioDatabase *portfolioDb = nullptr;
    AbstractCurrencyConverter *converter = nullptr;
};

#endif // EDITPORTFOLIOINTERACTOR_H
