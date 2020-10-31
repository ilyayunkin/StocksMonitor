#include "EditPortfolioInteractor.h"
#include "Entities/Entities.h"
#include "AbstractDialogs.h"
#include "AbstractPortfolioDatabase.h"
#include "AbstractCurrencyConverter.h"
#include "Subscriptions.h"
#include "Presenters/CurrencyPresenter.h"

EditPortfolioInteractor::EditPortfolioInteractor(Entities &entities,
                                                 Subscriptions &subscriptions)
    : entities(entities)
    , subscriptions(subscriptions)
{
}

void EditPortfolioInteractor::setDialogs(AbstractDialogs * const dialogs)
{
    this->dialogs = dialogs;
}

void EditPortfolioInteractor::setPortfolioDatabase(AbstractPortfolioDatabase *const portfolioDb)
{
    this->portfolioDb = portfolioDb;
    entities.portfolio = portfolioDb->getAll();
    for(stocksListHandler handler = 0; handler < entities.pairs.size(); ++handler)
    {
        const auto &pair = entities.pairs[handler];
        entities.registerStockSourceInPortfolio(pair.name, handler);
    }
}

void EditPortfolioInteractor::setConverter(AbstractCurrencyConverter * const converter)
{
    this->converter = converter;
}

void EditPortfolioInteractor::addToPortfolio(const stocksListHandler handler, const char *const ticker, const int quantity)
{
    auto entryIt = std::find_if(entities.portfolio.begin(), entities.portfolio.end(),
                                [&](const PortfolioEntry &e){return e.ticker == ticker;});
    const bool alreadyInPortfolio = entryIt != entities.portfolio.end();
    if(alreadyInPortfolio)
    {
        if(dialogs->askAddQuantityToPortfolio(entryIt->ticker.data()))
        {
            entryIt->quantity+= quantity;
            int row = entryIt - entities.portfolio.begin();
            portfolioDb->update(*entryIt);
            subscriptions.updatePortfolioView(row);
        }

        return;
    }else
    {
        const auto &pair = entities.pairs[handler];
        Stock stock = entities.getStock(handler, ticker);
        entities.portfolio.push_back(
                    PortfolioEntry{pair.name, stock.name, ticker, quantity,
                                   stock.price, 0, stock.price * quantity,
                                   pair.currencyCode, handler});
        portfolioDb->add(pair.name, ticker, quantity);
        subscriptions.updatePortfolioView();
    }
}

void EditPortfolioInteractor::deletePortfolioEntry(size_t row)
{
    auto ticker = entities.portfolio.at(row).ticker;
    std::remove_if(entities.portfolio.begin(), entities.portfolio.end(),
                   [&ticker](const PortfolioEntry &e){return ticker == e.ticker;});
    portfolioDb->deleteEntry(ticker.data());
    subscriptions.updatePortfolioView();
}

bool EditPortfolioInteractor::setPortfolioEntryQuantity(size_t row, int quantity)
{
    assert(quantity > 0);

    entities.portfolio[row].quantity = quantity;
    portfolioDb->update(entities.portfolio.at(row));
    subscriptions.updatePortfolioView();
    return true;
}

bool EditPortfolioInteractor::setPortfolioEntryReferencePrice(size_t row, float referencePrice)
{
    entities.portfolio[row].sellPrice = referencePrice;
    portfolioDb->update(entities.portfolio.at(row));
    subscriptions.updatePortfolioView();
    return true;
}

QString EditPortfolioInteractor::getPortfolioPrice(const char * const currency)
{
    assert(converter);
    return CurrencyPresenter::toText(converter->convert(currency, entities.getPortfolioSum()));
}

QString EditPortfolioInteractor::getPortfolioPrice()
{
    return CurrencyPresenter::toText(entities.getPortfolioSum());
}
