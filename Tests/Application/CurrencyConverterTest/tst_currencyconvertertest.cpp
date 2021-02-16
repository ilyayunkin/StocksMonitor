#include <QtTest>
#include "Application/CurrencyConverter.h"
#include "Rules/AbstractDialogs.h"
#include "Application/AbstractCurrencyCourseSource.h"
// add necessary includes here

struct CurrencySource : public AbstractCurrencyCourseSource
{
    mutable int askedTimes = 0;
    double getCurrencyCourse([[maybe_unused]]const char * const currency)const
    {
        ++askedTimes;
        return 2;
    }
};

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
        return 2.0;
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
    void doesntAskCourseIfSourceSpecified();
    void returnsTheSameValueForTheSameCurrency();
    void converts();
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
    QCOMPARE(dialogs.askedTimes, 0);
    converter.convert("RUB", counters);
    QCOMPARE(dialogs.askedTimes, 1);
    converter.convert("RUB", counters);
    QCOMPARE(dialogs.askedTimes, 2);
    converter.convert("USD", "RUB", 42);
    QCOMPARE(dialogs.askedTimes, 3);
}

void CurrencyConverterTest::doesntAskCourseIfSourceSpecified()
{
    Dialogs dialogs;
    CurrencySource source;
    CurrencyConverter converter("RUB", dialogs, &source);
    CurrencyCountersList counters;
    counters.add("USD", 1);
    QCOMPARE(dialogs.askedTimes, 0);
    converter.convert("RUB", counters);
    QCOMPARE(dialogs.askedTimes, 0);
}

void CurrencyConverterTest::returnsTheSameValueForTheSameCurrency()
{
    {
        const double val = 42;
        Dialogs dialogs;
        CurrencySource source;
        CurrencyConverter converter("RUB", dialogs, &source);
        CurrencyCountersList counters;
        counters.add("RUB", val);
        QCOMPARE(dialogs.askedTimes, 0);
        QCOMPARE(val, converter.convert("RUB", counters).list.front().sum);
        QCOMPARE(converter.convert("RUB", "RUB", val), val);
        QCOMPARE(dialogs.askedTimes, 0);
    }
    {
        const double val = 42;
        Dialogs dialogs;
        CurrencyConverter converter("RUB", dialogs);
        CurrencyCountersList counters;
        counters.add("RUB", val);
        QCOMPARE(dialogs.askedTimes, 0);
        QCOMPARE(val, converter.convert("RUB", counters).list.front().sum);
        QCOMPARE(converter.convert("RUB", "RUB", val), val);
        QCOMPARE(dialogs.askedTimes, 0);
    }
}

void CurrencyConverterTest::converts()
{
    {
        const double val = 42;
        const auto targetVal = val * 2;
        Dialogs dialogs;
        CurrencySource source;
        CurrencyConverter converter("RUB", dialogs, &source);
        CurrencyCountersList counters;
        counters.add("USD", val);
        QCOMPARE(dialogs.askedTimes, 0);
        QCOMPARE(targetVal, converter.convert("RUB", counters).list.front().sum);
        QCOMPARE(converter.convert("RUB", "USD", val), targetVal);
        QCOMPARE(dialogs.askedTimes, 0);
    }
    {
        const double val = 42;
        const auto targetVal = val / 2;
        Dialogs dialogs;
        CurrencySource source;
        CurrencyConverter converter("RUB", dialogs, &source);
        CurrencyCountersList counters;
        counters.add("RUB", val);
        QCOMPARE(dialogs.askedTimes, 0);
        QCOMPARE(targetVal, converter.convert("USD", counters).list.front().sum);
        QCOMPARE(converter.convert("USD", "RUB", val), targetVal);
        QCOMPARE(dialogs.askedTimes, 0);
    }
}

QTEST_APPLESS_MAIN(CurrencyConverterTest)

#include "tst_currencyconvertertest.moc"
