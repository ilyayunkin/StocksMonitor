#include "CurrencyCourseSource.h"
#include "StocksInterface.h"
#include "StocksInterface.h"

CurrencyCourseSource::CurrencyCourseSource(const StocksInterface &stocksInterface)
    : stocksInterface(stocksInterface)
{
}

double CurrencyCourseSource::getCurrencyCourse(const char * const currency) const
{
    return stocksInterface.getStock(currency).price;
}
