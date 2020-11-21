#ifndef ProcessStatisticsInteractor_H
#define ProcessStatisticsInteractor_H

#include "Entities/CurrencyCounter.h"
#include "StatisticsResults.h"
#include "Entities/Entities.h"

class Portfolio;
class AbstractCurrencyConverter;

class ProcessStatisticsInteractor
{
public:
    ProcessStatisticsInteractor(const Portfolio &portfolio,
                                const StatisticsConfigList &statisticsConfig);

    void setConverter(AbstractCurrencyConverter *const converter);
    Statistics processStatistics() const;
private:
    const Portfolio &portfolio;
    const StatisticsConfigList &statisticsConfig;
    AbstractCurrencyConverter * converter = nullptr;
    void processCategoies(Statistics &statistics) const;
};

#endif // ProcessStatisticsInteractor_H
