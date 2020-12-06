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

class ProcessStatisticsInteractorTest : public QObject
{
    Q_OBJECT
    Portfolio portfolio;
    StatisticsConfigList statisticsConfig;
    ProcessStatisticsInteractor interactor;
    CurrencyConverter converter;
public:
    ProcessStatisticsInteractorTest();
    ~ProcessStatisticsInteractorTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void processingThrowsOnEmptyPortfolio();
    void processingDoesntThrowOnEmptyConfig();
    void stocksThatDontFallIntoAnyGroupFallIntoUnknownGroup();
    void stocksFallIntoGroupSpecifiedTheirTickers();
    void statisticsIncludesCurrencyCategory();
    void statisticsIncludesPluginsCategory();
    void statisticsIncludesTrendCategory();
    void totalStatisticsIsEqualToSumOfGroupsStatistics();
};

ProcessStatisticsInteractorTest::ProcessStatisticsInteractorTest()
    : portfolio()
    , statisticsConfig()
    , interactor(portfolio, statisticsConfig)
{
    interactor.setConverter(&converter);
}

ProcessStatisticsInteractorTest::~ProcessStatisticsInteractorTest()
{

}

void ProcessStatisticsInteractorTest::initTestCase()
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

void ProcessStatisticsInteractorTest::cleanupTestCase()
{
    portfolio.portfolio.clear();
    statisticsConfig.clear();
}

void ProcessStatisticsInteractorTest::processingThrowsOnEmptyPortfolio()
{
    initTestCase();

    portfolio.portfolio.clear();
    QVERIFY(portfolio.empty());
    try {
        interactor.processStatistics();
        QVERIFY(false);
    } catch (...) {
        QVERIFY(true);
    }
}

void ProcessStatisticsInteractorTest::processingDoesntThrowOnEmptyConfig()
{
    initTestCase();

    statisticsConfig.clear();
    QVERIFY(statisticsConfig.empty());
    QVERIFY(!portfolio.empty());
    try {
        interactor.processStatistics();
        QVERIFY(true);
    } catch (...) {
        QVERIFY(false);
    }
}

void ProcessStatisticsInteractorTest::stocksThatDontFallIntoAnyGroupFallIntoUnknownGroup()
{
    cleanupTestCase();
    const auto plugin1 = "Plugin1";
    const auto plugin2 = "Plugin2";
    const auto plugin1C = "RUB";
    const auto plugin2C = "USD";
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock1"), "ST1", "", 1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock2"), "ST2", "", 1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock3"), "ST3", "", 1),
                               1,
                               plugin2C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock4"), "ST4", "", 1),
                               1,
                               plugin2C,
                               1));

    StatisticsCathegoryConfig cat1("Cat1");
    {
        StatisticsGroupConfig group1("G1");
        group1.list.push_back(StockId("ST2", ""));
        cat1.list.push_back(group1);
    }
    {
        StatisticsGroupConfig group1("G2");
        group1.list.push_back(StockId("ST4", ""));
        cat1.list.push_back(group1);
    }
    statisticsConfig.push_back(cat1);
    const auto result = interactor.processStatistics();
    const auto resultCatIt = std::find_if(result.list.begin(), result.list.end(),
                                          [&](const auto &category){return category.name == cat1.name;});
    QVERIFY(resultCatIt != result.list.end());
    const auto &resultCategory = *resultCatIt;
    const auto unknownIt = std::find_if(resultCategory.list.begin(), resultCategory.list.end(),
                                        [&](const auto &group){return group.name == "UNKNOWN";});
    QVERIFY(unknownIt != resultCategory.list.end());
    const auto &unknownGroup = *unknownIt;
    QCOMPARE(unknownGroup.tickers.size(), 2u);
    QVERIFY(std::find(unknownGroup.tickers.begin(), unknownGroup.tickers.end(), "ST1")
            != unknownGroup.tickers.end());
    QVERIFY(std::find(unknownGroup.tickers.begin(), unknownGroup.tickers.end(), "ST3")
            != unknownGroup.tickers.end());
}

void ProcessStatisticsInteractorTest::stocksFallIntoGroupSpecifiedTheirTickers()
{
    cleanupTestCase();
    const auto plugin1 = "Plugin1";
    const auto plugin2 = "Plugin2";
    const auto plugin1C = "RUB";
    const auto plugin2C = "USD";
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock1"), "ST1", "", 1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock2"), "ST2", "", 1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock3"), "ST3", "", 1),
                               1,
                               plugin2C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock4"), "ST4", "", 1),
                               1,
                               plugin2C,
                               1));

    StatisticsCathegoryConfig cat1("Cat1");

    StatisticsGroupConfig group1("G1");
    group1.list.push_back(StockId("ST2", ""));
    cat1.list.push_back(group1);

    StatisticsGroupConfig group2("G2");
    group2.list.push_back(StockId("ST4", ""));
    cat1.list.push_back(group2);

    statisticsConfig.push_back(cat1);
    const auto result = interactor.processStatistics();
    const auto resultCatIt = std::find_if(result.list.begin(), result.list.end(),
                                          [&](const auto &category){return category.name == cat1.name;});
    QVERIFY(resultCatIt != result.list.end());
    const auto &resultCategory = *resultCatIt;
    {
        const auto g1it = std::find_if(resultCategory.list.begin(), resultCategory.list.end(),
                                       [&](const auto &group){return group.name == group1.name;});
        QVERIFY(g1it != resultCategory.list.end());
        const auto &g1 = *g1it;
        QCOMPARE(g1.tickers.size(), 1u);
        QVERIFY(std::find(g1.tickers.begin(), g1.tickers.end(), "ST2")
                != g1.tickers.end());
    }
    {
        const auto g2it = std::find_if(resultCategory.list.begin(), resultCategory.list.end(),
                                       [&](const auto &group){return group.name == group2.name;});
        QVERIFY(g2it != resultCategory.list.end());
        const auto &g2 = *g2it;
        QCOMPARE(g2.tickers.size(), 1u);
        QVERIFY(std::find(g2.tickers.begin(), g2.tickers.end(), "ST4")
                != g2.tickers.end());
    }
}

void ProcessStatisticsInteractorTest::statisticsIncludesCurrencyCategory()
{
    cleanupTestCase();
    const auto plugin1 = "Plugin1";
    const auto plugin2 = "Plugin2";
    const auto plugin1C = "RUB";
    const auto plugin2C = "USD";
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock1"), "ST1", "", 1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock2"), "ST2", "", 1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock3"), "ST3", "", 1),
                               1,
                               plugin2C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock4"), "ST4", "", 1),
                               1,
                               plugin2C,
                               1));

    StatisticsCathegoryConfig cat1("Cat1");

    StatisticsGroupConfig group1("G1");
    group1.list.push_back(StockId("ST2", ""));
    cat1.list.push_back(group1);

    StatisticsGroupConfig group2("G2");
    group2.list.push_back(StockId("ST4", ""));
    cat1.list.push_back(group2);

    statisticsConfig.push_back(cat1);
    const auto result = interactor.processStatistics();
    const auto resultCatIt = std::find_if(result.list.begin(), result.list.end(),
                                          [&](const auto &category){return category.name == "Currency";});

    QVERIFY(resultCatIt != result.list.end());
    const auto &resultCategory = *resultCatIt;
    {
        const auto g1it = std::find_if(resultCategory.list.begin(), resultCategory.list.end(),
                                       [&](const auto &group){return group.name == plugin1C;});
        QVERIFY(g1it != resultCategory.list.end());
        const auto &g1 = *g1it;
        QCOMPARE(g1.tickers.size(), 2u);
        QVERIFY(std::find(g1.tickers.begin(), g1.tickers.end(), "ST1")
                != g1.tickers.end());
        QVERIFY(std::find(g1.tickers.begin(), g1.tickers.end(), "ST2")
                != g1.tickers.end());
    }
    {
        const auto g2it = std::find_if(resultCategory.list.begin(), resultCategory.list.end(),
                                       [&](const auto &group){return group.name == plugin2C;});
        QVERIFY(g2it != resultCategory.list.end());
        const auto &g2 = *g2it;
        QCOMPARE(g2.tickers.size(), 2u);
        QVERIFY(std::find(g2.tickers.begin(), g2.tickers.end(), "ST3")
                != g2.tickers.end());
        QVERIFY(std::find(g2.tickers.begin(), g2.tickers.end(), "ST4")
                != g2.tickers.end());
    }
}

void ProcessStatisticsInteractorTest::statisticsIncludesPluginsCategory()
{
    cleanupTestCase();
    const auto plugin1 = "Plugin1";
    const auto plugin2 = "Plugin2";
    const auto plugin1C = "RUB";
    const auto plugin2C = "USD";
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock1"), "ST1", "", 1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock2"), "ST2", "", 1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock3"), "ST3", "", 1),
                               1,
                               plugin2C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock4"), "ST4", "", 1),
                               1,
                               plugin2C,
                               1));

    StatisticsCathegoryConfig cat1("Cat1");

    StatisticsGroupConfig group1("G1");
    group1.list.push_back(StockId("ST2", ""));
    cat1.list.push_back(group1);

    StatisticsGroupConfig group2("G2");
    group2.list.push_back(StockId("ST4", ""));
    cat1.list.push_back(group2);

    statisticsConfig.push_back(cat1);
    const auto result = interactor.processStatistics();
    const auto resultCatIt = std::find_if(result.list.begin(), result.list.end(),
                                          [&](const auto &category){return category.name == "Plugins";});

    QVERIFY(resultCatIt != result.list.end());
    const auto &resultCategory = *resultCatIt;
    {
        const auto g1it = std::find_if(resultCategory.list.begin(), resultCategory.list.end(),
                                       [&](const auto &group){return group.name == plugin1;});
        QVERIFY(g1it != resultCategory.list.end());
        const auto &g1 = *g1it;
        QCOMPARE(g1.tickers.size(), 2u);
        QVERIFY(std::find(g1.tickers.begin(), g1.tickers.end(), "ST1")
                != g1.tickers.end());
        QVERIFY(std::find(g1.tickers.begin(), g1.tickers.end(), "ST2")
                != g1.tickers.end());
    }
    {
        const auto g2it = std::find_if(resultCategory.list.begin(), resultCategory.list.end(),
                                       [&](const auto &group){return group.name == plugin2;});
        QVERIFY(g2it != resultCategory.list.end());
        const auto &g2 = *g2it;
        QCOMPARE(g2.tickers.size(), 2u);
        QVERIFY(std::find(g2.tickers.begin(), g2.tickers.end(), "ST3")
                != g2.tickers.end());
        QVERIFY(std::find(g2.tickers.begin(), g2.tickers.end(), "ST4")
                != g2.tickers.end());
    }
}

void ProcessStatisticsInteractorTest::statisticsIncludesTrendCategory()
{
    cleanupTestCase();
    const auto plugin1 = "Plugin1";
    const auto plugin2 = "Plugin2";
    const auto plugin1C = "RUB";
    const auto plugin2C = "USD";
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock1"), "ST1", "", 1, +1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock2"), "ST2", "", 1, +0.23),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock3"), "ST3", "", 1, +0.00005),
                               1,
                               plugin2C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock4"), "ST4", "", 1, - 0.0001),
                               1,
                               plugin2C,
                               1));

    StatisticsCathegoryConfig cat1("Cat1");

    StatisticsGroupConfig group1("G1");
    group1.list.push_back(StockId("ST2", ""));
    cat1.list.push_back(group1);

    StatisticsGroupConfig group2("G2");
    group2.list.push_back(StockId("ST4", ""));
    cat1.list.push_back(group2);

    statisticsConfig.push_back(cat1);
    const auto result = interactor.processStatistics();
    const auto resultCatIt = std::find_if(result.list.begin(), result.list.end(),
                                          [&](const auto &category){return category.name == "Trend";});

    QVERIFY(resultCatIt != result.list.end());
    const auto &resultCategory = *resultCatIt;
    {
        const auto g1it = std::find_if(resultCategory.list.begin(), resultCategory.list.end(),
                                       [&](const auto &group){return group.name == "Positive";});
        QVERIFY(g1it != resultCategory.list.end());
        const auto &g1 = *g1it;
        QCOMPARE(g1.tickers.size(), 3u);
        QVERIFY(std::find(g1.tickers.begin(), g1.tickers.end(), "ST1")
                != g1.tickers.end());
        QVERIFY(std::find(g1.tickers.begin(), g1.tickers.end(), "ST2")
                != g1.tickers.end());
        QVERIFY(std::find(g1.tickers.begin(), g1.tickers.end(), "ST3")
                != g1.tickers.end());
    }
    {
        const auto g2it = std::find_if(resultCategory.list.begin(), resultCategory.list.end(),
                                       [&](const auto &group){return group.name == "Negative";});
        QVERIFY(g2it != resultCategory.list.end());
        const auto &g2 = *g2it;
        QCOMPARE(g2.tickers.size(), 1u);
        QVERIFY(std::find(g2.tickers.begin(), g2.tickers.end(), "ST4")
                != g2.tickers.end());
    }
}

void ProcessStatisticsInteractorTest::totalStatisticsIsEqualToSumOfGroupsStatistics()
{
    cleanupTestCase();
    const auto plugin1 = "Plugin1";
    const auto plugin2 = "Plugin2";
    const auto plugin1C = "RUB";
    const auto plugin2C = "USD";
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock1"), "ST1", "", 241, +34, -0.65, + 26, -0.5),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin1,
                               Stock(QString("Stock2"), "ST2", "", 681, +23, -9, +83, -1),
                               1,
                               plugin1C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock3"), "ST3", "", 2436, +0.00005, -56, + 7, -0.8),
                               1,
                               plugin2C,
                               1));
    portfolio.portfolio.push_back(
                PortfolioEntry(plugin2,
                               Stock(QString("Stock4"), "ST4", "", 1765, - 0.0001, -8, + 20, -34),
                               1,
                               plugin2C,
                               1));

    StatisticsCathegoryConfig cat1("Cat1");

    StatisticsGroupConfig group1("G1");
    group1.list.push_back(StockId("ST2", ""));
    cat1.list.push_back(group1);

    StatisticsGroupConfig group2("G2");
    group2.list.push_back(StockId("ST4", ""));
    cat1.list.push_back(group2);

    statisticsConfig.push_back(cat1);
    const auto result = interactor.processStatistics();
    for(const auto &cathegory : result.list)
    {
        float sum = 0;
        float derrivationDay = 0;
        float derrivationWeek = 0;
        float derrivationMonth = 0;
        float derrivationYear = 0;
        for(const auto &group : cathegory.list)
        {
            sum+= group.totalSum;
            derrivationDay+= group.totalDerivation;
            derrivationWeek+= group.totalDerivationWeek;
            derrivationMonth+= group.totalDerivationMonth;
            derrivationYear+= group.totalDerivationYear;
        }
        QVERIFY(abs(result.totalDerivation - derrivationDay) < cathegory.list.size());
        QVERIFY(abs(result.totalDerivationWeek - derrivationWeek) < cathegory.list.size());
        QVERIFY(abs(result.totalDerivationMonth - derrivationMonth) < cathegory.list.size());
        QVERIFY(abs(result.totalDerivationYear - derrivationYear) < cathegory.list.size());
        QVERIFY(abs(result.totalSum - sum) < cathegory.list.size());
    }
}

QTEST_APPLESS_MAIN(ProcessStatisticsInteractorTest)

#include "tst_processstatisticsinteractortest.moc"
