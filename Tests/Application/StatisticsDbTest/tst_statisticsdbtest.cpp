#include <QtTest>
#include <QDir>
#include <QDebug>

// add necessary includes here
#include "../../../src/Application/StatisticsConfigDatabase.h"
namespace{
void clean()
{
    QDir().remove(QString("StatisticsConfig") + ".sqlite");
}
}

class StatisticsDbTest : public QObject
{
    Q_OBJECT
public:
    StatisticsDbTest();
    ~StatisticsDbTest();

private slots:
    void test_AddCategory();
    void test_AddGroup();
    void test_AddItem();
    void test_RemoveItem();
    void test_RemoveGroup();
    void test_RemoveCategory();
};

StatisticsDbTest::StatisticsDbTest()
{

}

StatisticsDbTest::~StatisticsDbTest()
{

}

void StatisticsDbTest::test_AddCategory()
{
    clean();

    StatisticsConfigDatabase db;
    {
        auto all = db.getAllCategories();
        QVERIFY(all.isEmpty());
    }

    const char * categories[] = {"Category1", "Category2"};
    {
        int i = 0;
        for(auto const c : categories)
        {
            db.addCategory(c);

            auto all = db.getAllCategories();
            QVERIFY(!all.isEmpty());
            QCOMPARE(all.size() , i + 1);
            QCOMPARE(all[i] , c);
            ++i;
        }
    }
    {
        int i = 0;
        auto all = db.getAllCategories();
        QVERIFY(all.size() == sizeof(categories) / sizeof (*categories));
        for(auto const c : categories)
        {
            all = db.getAllCategories();
            QVERIFY(!all.isEmpty());
            QCOMPARE(all[i] , c);
            ++i;
        }
    }
}

void StatisticsDbTest::test_AddGroup()
{
    clean();

    StatisticsConfigDatabase db;
    {
        auto all = db.getAllCategories();
        QVERIFY(all.isEmpty());
    }

    const char * categories[] = {"Category1", "Category2"};
    {
        int i = 0;
        for(auto const c : categories)
        {
            db.addCategory(c);

            {
                auto all = db.getAllCategories();
                QVERIFY(!all.isEmpty());
                QCOMPARE(all.size() , i + 1);
                QCOMPARE(all[i] , c);
                ++i;
            }
        }
    }

    {
        const auto cat = categories[0];
        {
            auto all = db.getAllGroups(cat);
            QVERIFY(all.isEmpty());
        }

        const char * groups[] = {"g1", "g2"};
        {
            int i = 0;
            for(auto const g : groups)
            {

                db.addGroup(cat, g);

                {
                    auto all = db.getAllGroups(cat);
                    QVERIFY(!all.isEmpty());
                    QCOMPARE(all.size() , i + 1);
                    QCOMPARE(all[i] , g);
                }
                ++i;
            }
            {
                auto all = db.getAllGroups(cat);
                QCOMPARE(all.size() , sizeof(groups) / sizeof (*groups));
            }

        }
    }
    {
        const auto cat = categories[1];
        {
            auto all = db.getAllGroups(cat);
            QVERIFY(all.isEmpty());
        }
    }
}

void StatisticsDbTest::test_AddItem()
{
    clean();

    StatisticsConfigDatabase db;
    {
        auto all = db.getAllCategories();
        QVERIFY(all.isEmpty());
    }

    const char * categories[] = {"Category1", "Category2"};
    {
        int i = 0;
        for(auto const c : categories)
        {
            db.addCategory(c);

            {
                auto all = db.getAllCategories();
                QVERIFY(!all.isEmpty());
                QCOMPARE(all.size() , i + 1);
                QCOMPARE(all[i] , c);
                ++i;
            }
        }
    }

    {
        const auto cat = categories[0];
        {
            auto all = db.getAllGroups(cat);
            QVERIFY(all.isEmpty());
        }

        const char * groups[] = {"g1", "g2"};
        {
            int i = 0;
            for(auto const g : groups)
            {

                db.addGroup(cat, g);

                {
                    auto all = db.getAllGroups(cat);
                    QVERIFY(!all.isEmpty());
                    QCOMPARE(all.size() , i + 1);
                    QCOMPARE(all[i] , g);
                }
                ++i;
            }
            {
                auto all = db.getAllGroups(cat);
                QCOMPARE(all.size() , sizeof(groups) / sizeof (*groups));
            }
        }

        {
            const auto g = groups[0];
            const StockId items[] = {StockId("i1", "item1")
                                     , StockId("i2", "item2")
                                     , StockId("i2", "item2")};
            {
                int i = 0;
                for(auto const item : items)
                {

                    db.addItem(g, item);

                    {
                        auto all = db.getAllItems(g);
                        QVERIFY(!all.empty());
                        QCOMPARE(all.size() , i + 1);
                        QVERIFY(all[i] == item);
                    }
                    ++i;
                }
                {
                    auto all = db.getAllItems(g);
                    QCOMPARE(all.size() , sizeof(items) / sizeof (*items));
                }
            }
            {
                {
                    auto all = db.getAllItems(groups[1]);
                    QVERIFY(all.empty());
                }
            }
        }
    }
}

void StatisticsDbTest::test_RemoveItem()
{
    clean();

    StatisticsConfigDatabase db;

    const char * categories[] = {"Category1", "Category2"};
    for(auto const c : categories)
    {
        db.addCategory(c);
    }
    {
        const auto cat = categories[0];
        const char * groups[] = {"g1", "g2"};
        for(auto const g : groups)
        {
            db.addGroup(cat, g);
        }
        {
            const auto g = groups[0];
            const StockId items[] = {StockId("i1", "item1")
                                     , StockId("i2", "item2")
                                     , StockId("i3", "item3")};
            for(auto const item : items)
            {
                db.addItem(g, item);
            }
            {
                auto all = db.getAllItems(g);
                QCOMPARE(all.size() , sizeof(items) / sizeof (*items));
                QVERIFY(std::find(all.begin(), all.end(), items[1]) != all.end());
                QVERIFY(std::find(all.begin(), all.end(), items[0]) != all.end());
                QVERIFY(std::find(all.begin(), all.end(), items[2]) != all.end());
            }
            db.removeItem(g, items[1].ticker.data());
            {
                auto all = db.getAllItems(g);
                QCOMPARE(all.size() , sizeof(items) / sizeof (*items) - 1);
                QVERIFY(std::find(all.begin(), all.end(), items[1]) == all.end());
                QVERIFY(std::find(all.begin(), all.end(), items[0]) != all.end());
                QVERIFY(std::find(all.begin(), all.end(), items[2]) != all.end());
            }
        }
    }
}

void StatisticsDbTest::test_RemoveGroup()
{
    clean();

    StatisticsConfigDatabase db;

    const char * categories[] = {"Category1", "Category2"};
    {
        for(auto const c : categories)
        {
            db.addCategory(c);
        }
    }
    {
        const auto cat = categories[0];
        const char * groups[] = {"g1", "g2", "g3"};
        {
            for(auto const g : groups)
            {
                db.addGroup(cat, g);
            }
            {
                auto all = db.getAllGroups(cat);
                QCOMPARE(all.size() , sizeof(groups) / sizeof (*groups));
                QVERIFY(std::find(all.begin(), all.end(), groups[1]) != all.end());
                QVERIFY(std::find(all.begin(), all.end(), groups[0]) != all.end());
                QVERIFY(std::find(all.begin(), all.end(), groups[2]) != all.end());
            }
            {
                const auto g = groups[1];
                const StockId items[] = {StockId("i1", "item1")
                                         , StockId("i2", "item2")
                                         , StockId("i3", "item3")};
                for(auto const item : items)
                {
                    db.addItem(g, item);
                }
                {
                    db.addItem(groups[0], StockId("i4", "item4"));
                    auto all = db.getAllItems(groups[0]);
                    QCOMPARE(all.size() , 1);
                }
                {
                    db.addItem(groups[2], StockId("i5", "item5"));
                    auto all = db.getAllItems(groups[2]);
                    QCOMPARE(all.size() , 1);
                }
                {
                    auto all = db.getAllItems(g);
                    QCOMPARE(all.size() , sizeof(items) / sizeof (*items));
                    QVERIFY(std::find(all.begin(), all.end(), items[1]) != all.end());
                    QVERIFY(std::find(all.begin(), all.end(), items[0]) != all.end());
                    QVERIFY(std::find(all.begin(), all.end(), items[2]) != all.end());
                }
                db.removeGroup(cat, groups[1]);
                {// Check if group is removed
                    auto all = db.getAllGroups(cat);
                    QCOMPARE(all.size() , sizeof(groups) / sizeof (*groups) - 1);
                    QVERIFY(std::find(all.begin(), all.end(), groups[1]) == all.end());
                    QVERIFY(std::find(all.begin(), all.end(), groups[0]) != all.end());
                    QVERIFY(std::find(all.begin(), all.end(), groups[2]) != all.end());
                }
                {// Check if associated items are removed
                    auto all = db.getAllItems(g);
                    QCOMPARE(all.size() , 0);
                }
                {// Check if other items aren't removed
                    {
                        auto all = db.getAllItems(groups[0]);
                        QCOMPARE(all.size() , 1);
                    }
                    {
                        auto all = db.getAllItems(groups[2]);
                        QCOMPARE(all.size() , 1);
                    }
                }
            }
        }
    }
}

void StatisticsDbTest::test_RemoveCategory()
{
    clean();

    StatisticsConfigDatabase db;
    {
        auto all = db.getAllCategories();
        QVERIFY(all.isEmpty());
    }

    const char * categories[] = {"Category1", "Category2", "Category3"};
    {
        for(auto const c : categories)
        {
            db.addCategory(c);
        }
    }
    {
        auto all = db.getAllCategories();
        QCOMPARE(all.size() , sizeof(categories) / sizeof (*categories));
        QVERIFY(std::find(all.begin(), all.end(), categories[1]) != all.end());
        QVERIFY(std::find(all.begin(), all.end(), categories[0]) != all.end());
        QVERIFY(std::find(all.begin(), all.end(), categories[2]) != all.end());
    }
    const auto cat = categories[1];
    const char * groups[] = {"g1", "g2", "g3"};
    {
        for(auto const g : groups)
        {
            db.addGroup(cat, g);
        }
        auto all = db.getAllGroups(cat);
        QCOMPARE(all.size() , sizeof(groups) / sizeof (*groups));
    }
    {
        const auto g = groups[1];
        const StockId items[] = {StockId("i1", "item1")
                                 , StockId("i2", "item2")
                                 , StockId("i3", "item3")};
        for(auto const item : items)
        {
            db.addItem(g, item);
        }
        {
            auto all = db.getAllItems(g);
            QCOMPARE(all.size() , sizeof(items) / sizeof (*items));
            QVERIFY(std::find(all.begin(), all.end(), items[1]) != all.end());
            QVERIFY(std::find(all.begin(), all.end(), items[0]) != all.end());
            QVERIFY(std::find(all.begin(), all.end(), items[2]) != all.end());
        }
        db.addGroup(categories[0], "g4");
        {
            auto all = db.getAllGroups(categories[0]);
            QCOMPARE(all.size() , 1);
        }
        {
            db.addItem("g4", StockId("i4", "item4"));
            auto all = db.getAllItems("g4");
            QCOMPARE(all.size() , 1);
        }
    }
    db.removeCategory(categories[1]);
    {// Check if category is removed
        auto all = db.getAllCategories();
        QCOMPARE(all.size() , sizeof(categories) / sizeof (*categories) - 1);
        QVERIFY(std::find(all.begin(), all.end(), categories[1]) == all.end());
        QVERIFY(std::find(all.begin(), all.end(), categories[0]) != all.end());
        QVERIFY(std::find(all.begin(), all.end(), categories[2]) != all.end());
    }
    {// Check if all groups of the category are removed
        auto all = db.getAllGroups(cat);
        QCOMPARE(all.size() , 0);
    }
    {// Check if other groups aren't removed
        auto all = db.getAllGroups(categories[0]);
        QCOMPARE(all.size() , 1);
    }
    for(auto const g : groups)
    {// Check if associated items are removed
        auto all = db.getAllItems(g);
        QCOMPARE(all.size() , 0);
    }
    {// Check if other items aren't removed
        auto all = db.getAllItems("g4");
        QCOMPARE(all.size() , 1);
    }
}

QTEST_APPLESS_MAIN(StatisticsDbTest)

#include "tst_statisticsdbtest.moc"
