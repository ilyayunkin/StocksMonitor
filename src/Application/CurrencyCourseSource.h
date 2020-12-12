#ifndef CURRENCYCOURSESOURCE_H
#define CURRENCYCOURSESOURCE_H

#include "AbstractCurrencyCourseSource.h"

class StocksInterface;
class CurrencyCourseSource : public AbstractCurrencyCourseSource
{
public:
    CurrencyCourseSource(const StocksInterface &stocksInterface);
    double getCurrencyCourse(const char * const currency) override;
private:
    const StocksInterface &stocksInterface;
};

#endif // CURRENCYCOURSESOURCE_H
