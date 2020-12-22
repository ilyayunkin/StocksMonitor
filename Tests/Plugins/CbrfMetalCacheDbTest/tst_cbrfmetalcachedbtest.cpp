#include <QtTest>
#include "CbrfMetalPlugin/CbrfMetalCacheDb.h"

class CbrfMetalCacheDbTest : public QObject
{
    Q_OBJECT

public:
    CbrfMetalCacheDbTest();
    ~CbrfMetalCacheDbTest();

private slots:
    void newDbIsEmpty();
    void notEmptyAfterPushing();
    void size();
    void deletesOlderThanYearFromLastPushed();
    void getsNullIfNoDataOnTheDateOrOlder();
    void getsKnownDataOrOlder();
    void getsLastCached();
};

CbrfMetalCacheDbTest::CbrfMetalCacheDbTest(){
}

CbrfMetalCacheDbTest::~CbrfMetalCacheDbTest(){
}

void CbrfMetalCacheDbTest::newDbIsEmpty(){
    CbrfMetalCacheDb cacheDb;
    QVERIFY(cacheDb.empty());
}

void CbrfMetalCacheDbTest::notEmptyAfterPushing(){
    CbrfMetalCacheDb cacheDb;
    QVERIFY(cacheDb.empty());

    auto date = QDate::currentDate();
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{date, "1", 0., 0.});
    QVERIFY(!cacheDb.empty());
}

void CbrfMetalCacheDbTest::size(){
    CbrfMetalCacheDb cacheDb;
    QVERIFY(cacheDb.empty());

    auto date = QDate::currentDate();
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{date, "1", 0., 0.});
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{date, "1", 0., 0.});
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{date, "1", 0., 0.});
    QCOMPARE(cacheDb.size(), 3u);
}

void CbrfMetalCacheDbTest::deletesOlderThanYearFromLastPushed(){
    CbrfMetalCacheDb cacheDb;
    QVERIFY(cacheDb.empty());

    auto date = QDate::currentDate();
    auto oldDate = date.addDays(-365);
    auto newDate = date.addDays(+1);
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{date, "1", 0., 0.});
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{date, "1", 0., 0.});
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{oldDate, "1", 0., 0.});
    QCOMPARE(cacheDb.size(), 3u);
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{newDate, "1", 0., 0.});
    QCOMPARE(cacheDb.size(), 3u);
}

void CbrfMetalCacheDbTest::getsNullIfNoDataOnTheDateOrOlder(){
    CbrfMetalCacheDb cacheDb;
    auto date = QDate::currentDate();
    auto knownDate = date.addDays(-100);
    auto knownDateMinus1 = knownDate.addDays(-1);
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{date, "1", 0., 0.});
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{knownDate, "1", 0., 0.});

    auto line = cacheDb.getLine(knownDateMinus1, "1");
    QVERIFY(!line);
}

void CbrfMetalCacheDbTest::getsKnownDataOrOlder(){
    CbrfMetalCacheDb cacheDb;
    auto date = QDate::currentDate();
    auto knownDate = date.addDays(-100);
    auto knownDatePlus1 = knownDate.addDays(+1);
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{date, "1", 0., 0.});
    cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{knownDate, "1", 0., 0.});

    auto line = cacheDb.getLine(knownDate, "1");
    QVERIFY(line);
    QCOMPARE(line->date, knownDate);

    auto linePlus1 = cacheDb.getLine(knownDatePlus1, "1");
    QVERIFY(linePlus1);
    QCOMPARE(linePlus1->date, knownDate);
}

void CbrfMetalCacheDbTest::getsLastCached()
{
    CbrfMetalCacheDb cacheDb;
    {
        auto cached = cacheDb.getCached();
        QVERIFY(!cached.date.isValid());
    }
    {
        auto date = QDate::currentDate();
        auto dayAgo = date.addDays(-1);
        auto weekAgo = date.addDays(-7);
        auto mAgo = date.addDays(-31);
        auto yearAgo = date.addDays(-365);
        cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{yearAgo, "1", 9., 0.});
        cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{mAgo, "1", 8., 0.});
        cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{weekAgo, "1", 5., 0.});
        cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{dayAgo, "1", 2., 0.});
        cacheDb.pushNew(CbrfMetalCacheDb::CacheElement{date, "1", 1., 0.});
        auto cached = cacheDb.getCached();
        QCOMPARE(cached.date, date);
        QCOMPARE(cached.list.front().derivation, float(-1 / 2. * 100));
        QCOMPARE(cached.list.front().derivationWeek, float(-4 / 5. * 100));
        QCOMPARE(cached.list.front().derivationMonth, float(-7 / 8. * 100));
        QCOMPARE(cached.list.front().derivationYear, float(-8 / 9. * 100));
    }
}

QTEST_APPLESS_MAIN(CbrfMetalCacheDbTest)

#include "tst_cbrfmetalcachedbtest.moc"
