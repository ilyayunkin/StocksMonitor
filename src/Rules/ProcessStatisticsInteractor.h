#ifndef ProcessStatisticsInteractor_H
#define ProcessStatisticsInteractor_H

#include <QString>

#include "Entities/CurrencyCounter.h"
#include "StatisticsResults.h"

class Entities;
class AbstractCurrencyConverter;

class ProcessStatisticsInteractor
{
public:
    ProcessStatisticsInteractor(const Entities &entities);

    void setConverter(AbstractCurrencyConverter *const converter);
    Statistics processStatistics() const;
private:
    const Entities &entities;
    AbstractCurrencyConverter * converter = nullptr;
};

#endif // ProcessStatisticsInteractor_H
