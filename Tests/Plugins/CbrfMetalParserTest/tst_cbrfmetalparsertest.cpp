#include <QtTest>
#include "CbrfMetalPlugin/CbrfMetalParser.h"
// add necessary includes here

class CbrfMetalParserTest : public QObject
{
    Q_OBJECT

    QByteArray arr;
    CbrfMetalParser parser;
public:
    CbrfMetalParserTest();
    ~CbrfMetalParserTest();

private slots:
    void rightRowCount();
    void rightTickers();
    void rightPrices();
    void rightDerivation();
    void rightDerivationWeek();
    void rightDerivationMonth();
    void rightDerivationYear();
};

QByteArray sample(){
    return R"TEST(<?xml version="1.0" encoding="windows-1251"?>
           <Metall FromDate="20201212" ToDate="20201212" name="Precious metals quotations">
           <Record Date="12.12.2020" Code="1">
           <Buy>4310,63</Buy>
           <Sell>4310,63</Sell>
           </Record><Record Date="12.12.2020" Code="2">
           <Buy>56,15</Buy>
           <Sell>56,15</Sell></Record>
           <Record Date="12.12.2020" Code="3">
           <Buy>2402,56</Buy>
           <Sell>2402,56</Sell>
           </Record>
           <Record Date="12.12.2020" Code="4">
           <Buy>5470,42</Buy>
           <Sell>5470,42</Sell>
           </Record></Metall>)TEST";
}

CbrfMetalParserTest::CbrfMetalParserTest()
    : arr(sample())
{

}

CbrfMetalParserTest::~CbrfMetalParserTest()
{

}

void CbrfMetalParserTest::rightRowCount()
{
    std::vector<QByteArray> tickerts = {"AURUM", "SILVER", "PL", "PALAD"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);
    QCOMPARE(list.size(), tickerts.size());
}

void CbrfMetalParserTest::rightTickers()
{
    std::vector<QByteArray> tickerts = {"AURUM", "SILVER", "PL", "PALAD"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), tickerts.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].ticker.data(), tickerts[i]);
    }
}

void CbrfMetalParserTest::rightPrices()
{
    std::vector<float> prices = {4310.63, 56.15, 2402.56, 5470.42};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), prices.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].price, prices[i]);
    }
}

void CbrfMetalParserTest::rightDerivation()
{
    std::vector<float> derivation = {Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivation, derivation[i]);
    }
}

void CbrfMetalParserTest::rightDerivationWeek()
{
    std::vector<float> derivation = {Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationWeek, derivation[i]);
    }
}

void CbrfMetalParserTest::rightDerivationMonth()
{
    std::vector<float> derivation = {Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationMonth, derivation[i]);
    }
}

void CbrfMetalParserTest::rightDerivationYear()
{
    std::vector<float> derivation = {Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationYear, derivation[i]);
    }
}

QTEST_APPLESS_MAIN(CbrfMetalParserTest)

#include "tst_cbrfmetalparsertest.moc"
