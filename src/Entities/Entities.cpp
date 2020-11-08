#include "Entities.h"

float Entities::getStockPrice(const stocksListHandler handler, const char *const ticker)
{
    const auto &stocks = pairs[handler].stocks;

    float ret{};
    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        ret = cit->price;
    }
    return ret;
}

Stock Entities::getStock(const stocksListHandler handler, const size_t i) const
{
    const auto &stocks = pairs[handler].stocks;

    Stock ret{};
    if(i < stocks.size())
    {
        ret = stocks.at(i);
    }
    return ret;
}

Stock Entities::getStock(const stocksListHandler handler, const char *const ticker) const
{
    const auto &stocks = pairs[handler].stocks;

    Stock ret{};
    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        ret = *cit;
    }
    return ret;
}

size_t Entities::getStocksCount(const stocksListHandler handler) const
{
    const auto &stocks = pairs[handler].stocks;

    return stocks.size();
}

std::string Entities::getStocksActualizationTime(const stocksListHandler handler) const
{
    const auto &pair = pairs[handler];
    return pair.time;
}

StockLimit Entities::getStockBuyRequest(const stocksListHandler handler, const size_t i) const
{
    const auto &limits = pairs[handler].limits;

    StockLimit ret{};
    if(i < limits.size())
    {
        ret = limits.at(i);
    }
    return ret;
}

StockLimit Entities::getStockBuyRequest(const stocksListHandler handler, const char *const ticker) const
{
    const auto &limits = pairs[handler].limits;

    StockLimit ret{};
    auto cit = std::find_if(limits.begin(), limits.end(),
                            [&](const StockLimit &stock){return stock.ticker == ticker;});
    if(cit != limits.cend())
    {
        ret = *cit;
    }
    return ret;
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

std::string Entities::getUrl(const stocksListHandler handler, const char * const ticker) const
{
    return getStock(handler, ticker).url;
}

