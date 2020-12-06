#include <QtTest>
#include "Entities/Entities.h"
#include "StocksList.h"
#include "Rules/LoadStocksInteractor.h"
#include "Rules/Subscriptions.h"
// add necessary includes here

class LoadStocksInteractorTest : public QObject
{
    Q_OBJECT

public:
    LoadStocksInteractorTest();
    ~LoadStocksInteractorTest();

private slots:
    void benchmark();

};

LoadStocksInteractorTest::LoadStocksInteractorTest()
{

}

LoadStocksInteractorTest::~LoadStocksInteractorTest()
{

}

void LoadStocksInteractorTest::benchmark()
{
    constexpr auto stocksCount = 300;
    StocksList stocks1_1;
    StocksList stocks1_2;
    stocks1_1.reserve(stocksCount);
    stocks1_2.reserve(stocksCount);
    StocksList stocks2_1;
    StocksList stocks2_2;
    stocks2_1.reserve(stocksCount);
    stocks2_2.reserve(stocksCount);
    StocksList stocks3_1;
    StocksList stocks3_2;
    stocks3_1.reserve(stocksCount);
    stocks3_2.reserve(stocksCount);
    for(int i = 0; i < stocksCount; ++i){
        stocks1_1.push_back(Stock(QString("stock1_%1").arg(i)
                                  , QString("ST1_%1").arg(i).toLatin1().data()
                                  , ""
                                  , 1 , 1 , 1 , 1 , 1));
        stocks1_2.push_back(Stock(QString("stock1_%1").arg(i)
                                  , QString("ST1_%1").arg(i).toLatin1().data()
                                  , ""
                                  , 2 , 2 , 2 , 2 , 2));
        stocks2_1.push_back(Stock(QString("stock2_%1").arg(i)
                                  , QString("ST2_%1").arg(i).toLatin1().data()
                                  , ""
                                  , 1 , 1 , 1 , 1 , 1));
        stocks2_2.push_back(Stock(QString("stock2_%1").arg(i)
                                  , QString("ST2_%1").arg(i).toLatin1().data()
                                  , ""
                                  , 2 , 2 , 2 , 2 , 2));
        stocks3_1.push_back(Stock(QString("stock3_%1").arg(i)
                                  , QString("ST3_%1").arg(i).toLatin1().data()
                                  , ""
                                  , 1 , 1 , 1 , 1 , 1));
        stocks3_2.push_back(Stock(QString("stock3_%1").arg(i)
                                  , QString("ST3_%1").arg(i).toLatin1().data()
                                  , ""
                                  , 2 , 2 , 2 , 2 , 2));
    }
    constexpr auto limitsCount = 50;
    Entities entities;
    const auto plugin1 = "Plugin1";
    const auto pluginCurrency1 = "RUB";
    StockLimitsList limits1;
    for(int i = 0; i < limitsCount; ++i){
        limits1.push_back({stocks1_1[i], 1});
    }
    entities.pairs.push_back(StockListsPair{plugin1,
                                            pluginCurrency1,
                                            limits1});
    const auto plugin2 = "Plugin2";
    const auto pluginCurrency2 = "USD";
    StockLimitsList limits2;
    for(int i = 0; i < limitsCount; ++i){
        limits2.push_back({stocks2_1[i], 1});
    }
    entities.pairs.push_back(StockListsPair{plugin2,
                                            pluginCurrency2,
                                            limits2});
    const auto plugin3 = "Plugin1";
    const auto pluginCurrency3 = "EUR";
    StockLimitsList limits3;
    for(int i = 0; i < limitsCount; ++i){
        limits3.push_back({stocks3_1[i], 1});
    }
    entities.pairs.push_back(StockListsPair{plugin3,
                                            pluginCurrency3,
                                            limits3});

    entities.portfolio.registerStockSourceInPortfolio(plugin1, 0);
    entities.portfolio.registerStockSourceInPortfolio(plugin2, 1);
    entities.portfolio.registerStockSourceInPortfolio(plugin3, 2);

    for(int i = 0; i < limitsCount; ++i){
        entities.portfolio.portfolio.push_back(PortfolioEntry{plugin1, stocks1_1[i].ticker.data(), 1, 0});
        entities.portfolio.portfolio.push_back(PortfolioEntry{plugin2, stocks2_1[i].ticker.data(), 1, 0});
        entities.portfolio.portfolio.push_back(PortfolioEntry{plugin3, stocks3_1[i].ticker.data(), 1, 0});
    }

    Subscriptions subscriptions;
    LoadStocksInteractor interactor(entities, subscriptions);
    bool even = true;
    const auto time1 =  "time1";
    const auto time2 =  "time2";
    QBENCHMARK{
        const auto t = even ? time1 : time2;
        interactor.setStocks(0, StocksList{even ? stocks1_1 : stocks1_2}, t);
        interactor.setStocks(1, StocksList{even ? stocks2_1 : stocks2_2}, t);
        interactor.setStocks(2, StocksList{even ? stocks3_1 : stocks3_2}, t);
        even = !even;
    }
}

QTEST_APPLESS_MAIN(LoadStocksInteractorTest)

#include "tst_loadstocksinteractortest.moc"
