#include "RulesFasade.h"

#include <algorithm>
#include <set>
#include <assert.h>

#include <QDebug>

#include "AbstractCurrencyConverter.h"
#include "AbstractBuyRequestDatabase.h"
#include "Presenters/CurrencyPresenter.h"
#include "AbstractPortfolioDatabase.h"
#include "AbstractNotifier.h"

void RulesFasade::updateLimitsStorage(const stocksListHandler handler)
{
    auto &stockLimits = entities.pairs[handler].limits;

    int i = 0;
    bool anyUpdates = false;
    for(auto &limit : stockLimits)
    {
        float price = getStockPrice(handler, limit.ticker.data());
        if(price != limit.price)
        {
            anyUpdates = true;
            limit.price = price;
            if(price < limit.basePrice)
            {
                signalizeLimit(limit.name, price);
            }
        }
        ++i;
    }
    if(anyUpdates)
    {
        updateLimitsView(handler);
    }
}

void RulesFasade::updatePortfolioStorage(const stocksListHandler handler)
{
    const auto currency = entities.pairs[handler].currencyCode;
    bool needUpdate = false;
    for(auto &e : entities.portfolio)
    {
        if(e.handler == handler)
        {
            Stock stock = getStock(handler, e.ticker.data());
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
        updatePortfolioView();
    }
}

void RulesFasade::updateStocksView(const stocksListHandler handler)
{
    viewInterfaces[handler].stocks.update();
}

void RulesFasade::updateLimitsView(const stocksListHandler handler)
{
    viewInterfaces[handler].buyRequests.update();
}

void RulesFasade::updatePortfolioView()
{
    portfolioInterface.update();
}

void RulesFasade::updatePortfolioView(const size_t row)
{
    portfolioInterface.update(row);
}

void RulesFasade::registerStockSourceInPortfolio(const QString &name, const stocksListHandler handler)
{
    for(auto &e : entities.portfolio)
    {
        if(e.plugin == name)
        {
            e.handler = handler;
        }
    }
}

void RulesFasade::signalizePortfolio(const QString &name, const float price)
{
    if(notifier)
    {
        notifier->signalizePortfolio(name, price);
    }
}

void RulesFasade::signalizeLimit(const QString &name, const float price)
{
    if(notifier)
    {
        notifier->signalizeLimit(name, price);
    }
}

float RulesFasade::getStockPrice(const stocksListHandler handler, const char *const ticker)
{
    const auto &stocks = entities.pairs[handler].stocks;

    float ret{};
    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        ret = cit->price;
    }
    return ret;
}

Stock RulesFasade::getStock(const stocksListHandler handler, const char *const ticker) const
{
    const auto &stocks = entities.pairs[handler].stocks;

    Stock ret{};
    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        ret = *cit;
    }
    return ret;
}

size_t RulesFasade::getStocksCount(const stocksListHandler handler) const
{
    const auto &stocks = entities.pairs[handler].stocks;

    return stocks.size();
}

std::string RulesFasade::getStocksActualizationTime(const stocksListHandler handler) const
{
    const auto &pair = entities.pairs[handler];
    return pair.time;
}

StockLimit RulesFasade::getStockBuyRequest(const stocksListHandler handler, const size_t i) const
{
    const auto &limits = entities.pairs[handler].limits;

    StockLimit ret{};
    if(i < limits.size())
    {
        ret = limits.at(i);
    }
    return ret;
}

StockLimit RulesFasade::getStockBuyRequest(const stocksListHandler handler, const char *const ticker) const
{
    const auto &limits = entities.pairs[handler].limits;

    StockLimit ret{};
    auto cit = std::find_if(limits.begin(), limits.end(),
                            [&](const StockLimit &stock){return stock.ticker == ticker;});
    if(cit != limits.cend())
    {
        ret = *cit;
    }
    return ret;
}

size_t RulesFasade::getStockBuyRequestsCount(const stocksListHandler handler) const
{
    const auto &limits = entities.pairs[handler].limits;

    return limits.size();
}

void RulesFasade::addLimit(const stocksListHandler handler, const char * const ticker, float referencePrice)
{
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
            updateLimitsView(handler);
        }

        return;
    }else
    {
        Stock stock = getStock(handler, ticker);
        StockLimit newEntry{stock, referencePrice};
        stockLimits.push_back(newEntry);
        limitsDb->add(newEntry);
        updateLimitsView(handler);
    }
}

bool RulesFasade::setReferencePrice(const stocksListHandler handler, size_t row, float referencePrice)
{
    auto &pair = entities.pairs[handler];
    auto &limits = pair.limits;
    limits[row].basePrice = referencePrice;
    buyRequestDatabases[handler]->update(limits.at(row));
    return true;
}

void RulesFasade::addToPortfolio(const stocksListHandler handler, const char *const ticker, const int quantity)
{
    auto entryIt =
            std::find_if(entities.portfolio.begin(), entities.portfolio.end(),
                         [&](const PortfolioEntry &e){return e.ticker == ticker;});
    if(entryIt != entities.portfolio.end())
    {
        if(dialogs->askAddQuantityToPortfolio(entryIt->ticker.data()))
        {
            entryIt->quantity+= quantity;
            int row = entryIt - entities.portfolio.begin();
            portfolioDb->update(*entryIt);
            updatePortfolioView(row);
        }

        return;
    }else
    {
        const auto &pair = entities.pairs[handler];
        Stock stock = getStock(handler, ticker);
        entities.portfolio.push_back(PortfolioEntry{pair.name, stock.name, ticker, quantity,
                                                    stock.price, 0, stock.price * quantity,
                                                    pair.currencyCode, handler});
        portfolioDb->add(pair.name, ticker, quantity);
        updatePortfolioStorage(handler);
        portfolioInterface.update();
        updatePortfolioView();
    }
}

void RulesFasade::deletePortfolioEntry(size_t row)
{
    auto ticker = entities.portfolio.at(row).ticker;
    std::remove_if(entities.portfolio.begin(), entities.portfolio.end(),
                   [&ticker](const PortfolioEntry &e){return ticker == e.ticker;});
    portfolioDb->deleteEntry(ticker.data());
    portfolioInterface.update();
}

Stock RulesFasade::getStock(const stocksListHandler handler, const size_t i) const
{
    const auto &stocks = entities.pairs[handler].stocks;

    Stock ret{};
    if(i < stocks.size())
    {
        ret = stocks.at(i);
    }
    return ret;
}
CurrencyCountersList RulesFasade::getPortfolioSum() const
{
    CurrencyCountersList counters;
    for(auto &e : entities.portfolio)
    {
        counters.add(e.currency.data(), e.price * e.quantity);
    }

    return counters;
}

RulesFasade::RulesFasade() :
    portfolioInterface(this)
{
    qDebug() << __PRETTY_FUNCTION__;
}

stocksListHandler RulesFasade::addStocksSource(const StocksSource &source)
{
    stocksListHandler handler = entities.pairs.size();

    entities.pairs.push_back(StockListsPair{source.name,
                                            source.currencyCode,
                                            source.db->getAll()});
    buyRequestDatabases.push_back(source.db);
    viewInterfaces.push_back(ViewInterfacesPair(source.name, this, handler));
    registerStockSourceInPortfolio(source.name, handler);
    return handler;
}

void RulesFasade::setConverter(AbstractCurrencyConverter * const converter)
{
    this->converter = converter;
}

void RulesFasade::setNotifier(AbstractNotifier *const notifier)
{
    this->notifier = notifier;
}

void RulesFasade::setPortfolioDatabase(AbstractPortfolioDatabase *const portfolioDb)
{
    this->portfolioDb = portfolioDb;
    entities.portfolio = portfolioDb->getAll();
    for(stocksListHandler handler = 0; handler < entities.pairs.size(); ++handler)
    {
        const auto &pair = entities.pairs[handler];
        registerStockSourceInPortfolio(pair.name, handler);
    }
}

void RulesFasade::setDialogs(AbstractDialogs * const dialogs)
{
    this->dialogs = dialogs;
}

size_t RulesFasade::getPortfolioSize() const
{
    return entities.portfolio.size();
}

PortfolioEntry RulesFasade::getPortfolioEntry(const size_t i) const
{
    return entities.portfolio.at(i);
}

Stock RulesFasade::getStockForPortfolioEntry(const size_t i) const
{
    auto const &entry = entities.portfolio.at(i);
    return getStock(entry.handler, entry.ticker.data());
}

bool RulesFasade::setPortfolioEntryQuantity(size_t row, int quantity)
{
    if(quantity == 0)
    {
        const auto ticker = entities.portfolio[row].ticker;
        if(dialogs->askDeleteFromPortfolio(ticker.data()))
        {
            portfolioDb->deleteEntry(ticker.data());
        }
        return true;
    }if(quantity > 0)
    {
        entities.portfolio[row].quantity = quantity;
        portfolioDb->update(entities.portfolio.at(row));
        portfolioInterface.update();
        return true;
    }else
    {
        return false;
    }
}

bool RulesFasade::setPortfolioEntryReferencePrice(size_t row, float referencePrice)
{
    entities.portfolio[row].sellPrice = referencePrice;
    portfolioDb->update(entities.portfolio.at(row));
    portfolioInterface.update();
    return true;
}

RulesFasade::~RulesFasade()
{
    qDebug() << __PRETTY_FUNCTION__;
}

ViewInterfaces &RulesFasade::getViewInterfaces()
{
    return viewInterfaces;
}

PortfolioInterface &RulesFasade::getPortfolioInterface()
{
    return portfolioInterface;
}

QString RulesFasade::getPortfolioPrice(const char * const currency)
{
    assert(converter);
    return CurrencyPresenter::toText(converter->convert(currency, getPortfolioSum()));
}

QString RulesFasade::getPortfolioPrice()
{
    return CurrencyPresenter::toText(getPortfolioSum());
}

QStringList RulesFasade::getAvailibleCurrencies()
{
    auto counters = getPortfolioSum();

    assert(!counters.list.empty());

    QStringList items;
    std::transform(counters.list.begin(), counters.list.end(),
                   std::back_inserter(items),
                   [](const CurrencyCounter &c){return c.currency.data();});

    return items;
}

void RulesFasade::setStocks(const stocksListHandler handler,
                            StocksList &&stocks,
                            const char *const time)
{
    auto &pair = entities.pairs[handler];
    if(time != pair.time)
    {
        std::swap(pair.stocks, stocks);
        pair.time = time;

        updateLimitsStorage(handler);
        updatePortfolioStorage(handler);
        updateStocksView(handler);
    }
}
