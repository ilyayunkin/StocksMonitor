#include "Entities.h"

float Entities::getStockPrice(const stocksListHandler handler, const char *const ticker)
{
    const auto &stocks = pairs[handler].stocks;

    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        return  cit->price;
    }
    return 0.;
}

Stock Entities::getStock(const stocksListHandler handler, const size_t i) const
{
    const auto &stocks = pairs[handler].stocks;

    if(i < stocks.size())
    {
        return stocks.at(i);
    }
    return Stock{};
}

Stock Entities::getStock(const stocksListHandler handler, const char *const ticker) const
{
    const auto &stocks = pairs[handler].stocks;

    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        return *cit;
    }
    return Stock{};
}

size_t Entities::getStocksCount(const stocksListHandler handler) const
{
    const auto &stocks = pairs[handler].stocks;

    return stocks.size();
}

TimeString Entities::getStocksActualizationTime(const stocksListHandler handler) const
{
    const auto &pair = pairs[handler];
    return pair.time;
}

StockLimit Entities::getStockBuyRequest(const stocksListHandler handler, const size_t i) const
{
    const auto &limits = pairs[handler].limits;

    if(i < limits.size())
    {
        return limits.at(i);
    }
    return StockLimit{};
}

StockLimit Entities::getStockBuyRequest(const stocksListHandler handler, const char *const ticker) const
{
    const auto &limits = pairs[handler].limits;

    auto cit = std::find_if(limits.begin(), limits.end(),
                            [&](const StockLimit &stock){return stock.ticker == ticker;});
    if(cit != limits.cend())
    {
        return *cit;
    }
    return StockLimit{};
}

size_t Entities::getStockBuyRequestsCount(const stocksListHandler handler) const
{
    const auto &limits = pairs[handler].limits;

    return limits.size();
}

Stock Entities::getStockForPortfolioEntry(const size_t i) const
{
    auto const &entry = portfolio.portfolio.at(i);
    return getStock(entry.handler, entry.ticker.data());
}

Url Entities::getUrl(const stocksListHandler handler, const char * const ticker) const
{
    return getStock(handler, ticker).url;
}

