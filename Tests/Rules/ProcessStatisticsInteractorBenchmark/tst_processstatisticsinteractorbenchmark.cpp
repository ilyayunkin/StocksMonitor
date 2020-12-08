#include <QtTest>
#include "Rules/ProcessStatisticsInteractor.h"
#include "Entities/Statistics.h"
#include "Entities/Portfolio.h"
#include "Rules/AbstractCurrencyConverter.h"
// add necessary includes here

#include <cstring>

class CurrencyConverter : public AbstractCurrencyConverter
{
public:
    CurrencyCountersList convert(
            const char * const targetCurrency,
            const CurrencyCountersList &counters)const{
        CurrencyCountersList ret;
        for(auto &currency : counters.list)
        {
            ret.add(targetCurrency, convert(targetCurrency, currency.currency.data(), currency.sum));
        }
        return ret;
    }
    float convert(const char * const targetCurrency, const char * const currency, const float value)const{
        if(!strcmp(targetCurrency, currency)){
            return value;
        }else if(QString(targetCurrency) == "RUB"){
            return value * 2;
        }else{
            return value / 2;
        }
    }
};

class ProcessStatisticsInteractorBenchmark : public QObject
{
    Q_OBJECT

    Portfolio portfolio;
    StatisticsConfigList statisticsConfig;
    ProcessStatisticsInteractor interactor;
    CurrencyConverter converter;
    void initTestCase();
    void cleanupTestCase();
public:
    ProcessStatisticsInteractorBenchmark();
    ~ProcessStatisticsInteractorBenchmark();

private slots:
    void processStatisticsEmptyConfigBenchmark();
    void processStatisticsBenchmark();
};

void ProcessStatisticsInteractorBenchmark::initTestCase()
{
    cleanupTestCase();
    const auto plugin1 = "Plugin1";
    const auto plugin2 = "Plugin2";
    const auto plugin1C = "RUB";
    const auto plugin2C = "USD";
    {
        for(int i = 0; i < 10; ++i)
        {
            auto n = i;
            portfolio.portfolio.push_back(
                        PortfolioEntry(plugin1,
                                       Stock(QString("Stock%1").arg(n), QString("ST%1").arg(n).toLatin1().data(), "", 1),
                                       i,
                                       plugin1C,
                                       1));
        }
        for(int i = 0; i < 10; ++i)
        {
            auto n = i + 100;
            portfolio.portfolio.push_back(
                        PortfolioEntry(plugin2,
                                       Stock(QString("Stock%1").arg(n), QString("ST%1").arg(n).toLatin1().data(), "", 1),
                                       i,
                                       plugin2C,
                                       2));
        }
    }
    StatisticsCathegoryConfig cat1("Cat1");
    {
        StatisticsGroupConfig group1("G1");
        for(int i = 0; i < 80; ++i)
        {
            group1.list.push_back(
                        StockId(QString("ST%1").arg(i).toLatin1().data(), ""));
        }
        cat1.list.push_back(group1);
    }
    {
        StatisticsGroupConfig group1("G2");
        for(int i = 95; i < 120; ++i)
        {
            group1.list.push_back(
                        StockId(QString("ST%1").arg(i + 40).toLatin1().data(), ""));
        }
        cat1.list.push_back(group1);
    }
    statisticsConfig.push_back(cat1);
}

void ProcessStatisticsInteractorBenchmark::cleanupTestCase()
{
    portfolio.portfolio.clear();
    statisticsConfig.clear();
}

ProcessStatisticsInteractorBenchmark::ProcessStatisticsInteractorBenchmark()
    : portfolio()
    , statisticsConfig()
    , interactor(portfolio, statisticsConfig, converter)
{
}

ProcessStatisticsInteractorBenchmark::~ProcessStatisticsInteractorBenchmark()
{

}

void ProcessStatisticsInteractorBenchmark::processStatisticsEmptyConfigBenchmark()
{
    initTestCase();

    statisticsConfig.clear();
    QBENCHMARK{
        interactor.processStatistics();
    }
}

void ProcessStatisticsInteractorBenchmark::processStatisticsBenchmark()
{
    initTestCase();

    QBENCHMARK{
        interactor.processStatistics();
    }
}

QTEST_APPLESS_MAIN(ProcessStatisticsInteractorBenchmark)

#include "tst_processstatisticsinteractorbenchmark.moc"
