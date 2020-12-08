#include "EditPortfolioInteractor.h"
#include "Entities/Entities.h"
#include "AbstractDialogs.h"
#include "AbstractPortfolioDatabase.h"
#include "AbstractCurrencyConverter.h"
#include "Subscriptions.h"
#include "Presenters/CurrencyPresenter.h"

EditPortfolioInteractor::EditPortfolioInteractor(Entities &entities,
                                                 Subscriptions &subscriptions,
                                                 const AbstractCurrencyConverter &converter)
    : entities(entities)
    , subscriptions(subscriptions)
    , converter(converter)
{
}

void EditPortfolioInteractor::setDialogs(AbstractDialogs * const dialogs)
{
    this->dialogs = dialogs;
}

void EditPortfolioInteractor::setPortfolioDatabase(AbstractPortfolioDatabase *const portfolioDb)
{
    this->portfolioDb = portfolioDb;
    entities.portfolio.portfolio = portfolioDb->getAll();
    for(stocksListHandler handler = 0; handler < entities.pairs.size(); ++handler)
    {
        const auto &pair = entities.pairs[handler];
        entities.portfolio.registerStockSourceInPortfolio(pair.name, handler);
    }
}

void EditPortfolioInteractor::addToPortfolio(const stocksListHandler handler, const char *const ticker, const int quantity)
{
    auto entryIt = std::find_if(entities.portfolio.portfolio.begin(), entities.portfolio.portfolio.end(),
                                [&](const PortfolioEntry &e){return e.ticker == ticker;});
    const bool alreadyInPortfolio = entryIt != entities.portfolio.portfolio.end();
    if(alreadyInPortfolio)
    {
        if(dialogs->askAddQuantityToPortfolio(entryIt->ticker.data()))
        {
            entryIt->quantity+= quantity;
            int row = entryIt - entities.portfolio.portfolio.begin();
            portfolioDb->update(*entryIt);
            subscriptions.updatePortfolioView(row);
        }

        return;
    }else
    {
        const auto &pair = entities.pairs[handler];
        Stock stock = entities.getStock(handler, ticker);
        entities.portfolio.portfolio.push_back(
                    PortfolioEntry{pair.name, stock, quantity,
                                   pair.currencyCode, handler});
        portfolioDb->add(pair.name, ticker, quantity);
        subscriptions.updatePortfolioView();
    }
}

void EditPortfolioInteractor::deletePortfolioEntry(size_t row)
{
    const auto ticker = entities.portfolio.portfolio.at(row).ticker;
    const auto afterEnd = std::remove_if(entities.portfolio.portfolio.begin(),
                                   entities.portfolio.portfolio.end(),
                                   [&ticker](const PortfolioEntry &e){return ticker == e.ticker;});
    if(afterEnd != entities.portfolio.portfolio.end())
        entities.portfolio.portfolio.erase(afterEnd, entities.portfolio.portfolio.end());
    portfolioDb->deleteEntry(ticker.data());
    subscriptions.updatePortfolioView();
}

bool EditPortfolioInteractor::setPortfolioEntryQuantity(size_t row, int quantity)
{
    assert(quantity > 0);

    entities.portfolio.portfolio[row].quantity = quantity;
    portfolioDb->update(entities.portfolio.portfolio.at(row));
    subscriptions.updatePortfolioView();
    return true;
}

bool EditPortfolioInteractor::setPortfolioEntryReferencePrice(size_t row, float referencePrice)
{
    entities.portfolio.portfolio[row].sellPrice = referencePrice;
    portfolioDb->update(entities.portfolio.portfolio.at(row));
    subscriptions.updatePortfolioView();
    return true;
}

QString EditPortfolioInteractor::getPortfolioPrice(const char * const currency)
{
    return CurrencyPresenter::toText(converter.convert(currency, entities.portfolio.sum()));
}

QString EditPortfolioInteractor::getPortfolioPrice()
{
    return CurrencyPresenter::toText(entities.portfolio.sum());
}

