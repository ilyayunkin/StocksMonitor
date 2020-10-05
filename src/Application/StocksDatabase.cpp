#include "StocksDatabase.h"

#include <QBrush>

#include <algorithm>

StocksDatabase::StocksDatabase(const QString &plugin, const QByteArray &currencyCode) :
    _pluginName(plugin),
    _currencyCode(currencyCode)
{
}

QString StocksDatabase::pluginName() const
{
    return _pluginName;
}

QByteArray StocksDatabase::currencyCode() const
{
    return _currencyCode;
}

void StocksDatabase::setStocks(StocksList &&stocks)
{
    this->stocks = stocks;
    if(view != nullptr)
    {
        view->stocksUpdated();
    }
}

Stock StocksDatabase::getStock(const QByteArray &ticker) const
{
    Stock ret{};
    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        ret = *cit;
    }
    return ret;
}

Stock StocksDatabase::getStock(const size_t i) const
{
    Stock ret{};
    if(i < stocks.size())
    {
        ret = stocks.at(i);
    }
    return ret;
}

float StocksDatabase::getStockPrice(const QByteArray &ticker) const
{
    float ret{};
    auto cit = std::find_if(stocks.begin(), stocks.end(),
                            [&](const Stock &stock){return stock.ticker == ticker;});
    if(cit != stocks.cend())
    {
        ret = cit->price;
    }
    return ret;
}

void StocksDatabase::setView(AbstractStocksView *view)
{
    this->view = view;
}
