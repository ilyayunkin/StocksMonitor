#include <QtTest>
#include "CbrfCurrencyPlugin/CbrfCurrencyParser.h"
// add necessary includes here

class CbrfCurrencyParserTest : public QObject
{
    Q_OBJECT
    QByteArray arr;
    CbrfCurrencyParser parser;
public:
    CbrfCurrencyParserTest();
    ~CbrfCurrencyParserTest();

private slots:
    void rightRowCount();
    void rightTickers();
    void rightPrices();
    void rightDerivation();
    void rightDerivationWeek();
    void rightDerivationMonth();
    void rightDerivationYear();
    void dateFromXmlReturnedAsTime();
};
QByteArray sample(){
    return R"TEST(<?xml version="1.0" encoding="windows-1251"?>
           <ValCurs Date="12.12.2020" name="Foreign Currency Market">
           <Valute ID="R01010">
           <NumCode>036</NumCode>
           <CharCode>AUD</CharCode>
           <Nominal>1</Nominal>
           <Name>Австралийский доллар</Name>
           <Value>55,0955</Value></Valute>
           <Valute ID="R01020A">
           <NumCode>944</NumCode>
           <CharCode>AZN</CharCode>
           <Nominal>1</Nominal>
           <Name>Азербайджанский манат</Name>
           <Value>43,0368</Value></Valute>
           <Valute ID="R01035">
           <NumCode>826</NumCode>
           <CharCode>GBP</CharCode>
           <Nominal>1</Nominal>
           <Name>Фунт стерлингов Соединенного королевства</Name>
           <Value>96,7371</Value></Valute>
           <Valute ID="R01060">
           <NumCode>051</NumCode>
           <CharCode>AMD</CharCode>
           <Nominal>100</Nominal>
           <Name>Армянских драмов</Name>
           <Value>14,2131</Value></Valute>
           <Valute ID="R01090B">
           <NumCode>933</NumCode>
           <CharCode>BYN</CharCode>
           <Nominal>1</Nominal>
           <Name>Белорусский рубль</Name>
           <Value>28,8303</Value></Valute></ValCurs>)TEST";
}

CbrfCurrencyParserTest::CbrfCurrencyParserTest()
    : arr(sample())
{

}

CbrfCurrencyParserTest::~CbrfCurrencyParserTest()
{

}

void CbrfCurrencyParserTest::rightRowCount()
{
    std::vector<QByteArray> tickerts = {"AUD", "AZN", "GBP", "AMD", "BYN"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);
    QCOMPARE(list.size(), tickerts.size());
}

void CbrfCurrencyParserTest::rightTickers()
{
    std::vector<QByteArray> tickerts = {"AUD", "AZN", "GBP", "AMD", "BYN"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), tickerts.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].ticker.data(), tickerts[i]);
    }
}

void CbrfCurrencyParserTest::rightPrices()
{
    std::vector<float> prices = {55.0955, 43.0368, 96.7371, 14.2131, 28.8303};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), prices.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].price, prices[i]);
    }
}

void CbrfCurrencyParserTest::rightDerivation()
{
    std::vector<float> derivation = {Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivation, derivation[i]);
    }
}

void CbrfCurrencyParserTest::rightDerivationWeek()
{
    std::vector<float> derivation = {Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationWeek, derivation[i]);
    }
}

void CbrfCurrencyParserTest::rightDerivationMonth()
{
    std::vector<float> derivation = {Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationMonth, derivation[i]);
    }
}

void CbrfCurrencyParserTest::rightDerivationYear()
{
    std::vector<float> derivation = {Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation, Stock::defaultDerivation};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationYear, derivation[i]);
    }
}

void CbrfCurrencyParserTest::dateFromXmlReturnedAsTime()
{
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);
    QCOMPARE(QString(time.data()), "12.12.2020");
}


QTEST_APPLESS_MAIN(CbrfCurrencyParserTest)

#include "tst_cbrfcurrencyparsertest.moc"
