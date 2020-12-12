#include <QtTest>
#include "Application/CurrencyConverter.h"
#include "Rules/AbstractDialogs.h"
// add necessary includes here

struct Dialogs : public AbstractDialogs
{
    mutable int askedTimes = 0;

    bool askReplaceBuyRequest([[maybe_unused]]const char * const ticker, [[maybe_unused]]const float sellPrice) const
    {return true;};
    bool askDeleteBuyRequest([[maybe_unused]]const char * const ticker) const
    {return true;};
    bool askDeleteFromPortfolio([[maybe_unused]]const char * const ticker) const
    {return true;};
    bool askAddQuantityToPortfolio([[maybe_unused]]const char * const ticker) const
    {return true;};
    bool askReplaceItemFromStatistics([[maybe_unused]]const char * const ticker) const
    {return true;};
    bool askReplaceGroupStatistics([[maybe_unused]]const QString &name) const
    {return true;};
    bool askReplaceCategoryStatistics([[maybe_unused]]const QString &name) const
    {return true;};
    double askCurrencyCourse([[maybe_unused]]const char * const currency, [[maybe_unused]]const char * const targetCurrency) const
    {
        ++askedTimes;
        return true;
    };
    double getBuyRequestPrice([[maybe_unused]]StockName name, [[maybe_unused]]float price, [[maybe_unused]]bool *ok) const
    {return 1;}
};

class CurrencyConverterTest : public QObject
{
    Q_OBJECT
public:
    CurrencyConverterTest();
    ~CurrencyConverterTest();

private slots:
    void asksCurrencyCourseIfCurrencySourceIsntSpecified();
    void returnsTheSameValueForTheSameCurrency();
};

CurrencyConverterTest::CurrencyConverterTest()
{

}

CurrencyConverterTest::~CurrencyConverterTest()
{

}

void CurrencyConverterTest::asksCurrencyCourseIfCurrencySourceIsntSpecified()
{
    Dialogs dialogs;
    CurrencyConverter converter("RUB", dialogs);
    CurrencyCountersList counters;
    counters.add("USD", 1);
    QVERIFY(!dialogs.askedTimes);
    converter.convert("RUB", counters);
    QCOMPARE(dialogs.askedTimes, 1);
    converter.convert("RUB", counters);
    QCOMPARE(dialogs.askedTimes, 2);
}

void CurrencyConverterTest::returnsTheSameValueForTheSameCurrency()
{
    const double val = 42;
    Dialogs dialogs;
    CurrencyConverter converter("RUB", dialogs);
    CurrencyCountersList counters;
    counters.add("RUB", val);
    QCOMPARE(val, converter.convert("RUB", counters).list.front().sum);
    QCOMPARE(converter.convert("RUB", "RUB", val), val);
    QCOMPARE(dialogs.askedTimes, 0);
}

QTEST_APPLESS_MAIN(CurrencyConverterTest)

#include "tst_currencyconvertertest.moc"
