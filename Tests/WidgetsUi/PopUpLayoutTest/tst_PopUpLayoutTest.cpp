#include <QtTest>
#include <QCoreApplication>

#include "WidgetsUi/PopUp/PopUpLayout.h"
// add necessary includes here
struct Widget : public AbstractPopUpWindow
{
    QRect r;
    int width()const override{return r.width();}
    int height()const override{return r.height();}
    int x()const override{return r.x();}
    int y()const override{return r.y();}
    void setGeometry(int x, int y, int w, int h) override{r = QRect(x, y, w, h);}
};

class PopUpLayoutTest : public QObject
{
    Q_OBJECT

public:
    PopUpLayoutTest();
    ~PopUpLayoutTest();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_PlacesFirstWidgetInTheBottomRightCorner();
    void test_PlacesSecondWidgetAboveFirst();
    void test_MovesWidgetsDownAfterFirstRemoved();
};

PopUpLayoutTest::PopUpLayoutTest()
{
}

PopUpLayoutTest::~PopUpLayoutTest()
{
}

void PopUpLayoutTest::initTestCase()
{
}

void PopUpLayoutTest::cleanupTestCase()
{
}

void PopUpLayoutTest::test_PlacesFirstWidgetInTheBottomRightCorner()
{
    QRect window(0, 0, 1280, 960);
    PopUpLayout layout(window);

    auto const span = 40;
    auto const width = 200;
    auto const height = 100;
    auto *w = new Widget;
    w->setGeometry(0, 0, width, height);
    QCOMPARE(w->height(), height);
    QCOMPARE(w->width(), width);
    QCOMPARE(w->x(), 0);
    QCOMPARE(w->y(), 0);

    layout.place(w);

    QCOMPARE(w->y(), window.height() - span - height);
}

void PopUpLayoutTest::test_PlacesSecondWidgetAboveFirst()
{
    QRect window(0, 0, 1280, 960);
    PopUpLayout layout(window);

    auto const span = 40;

    auto const width = 200;
    auto const height = 110;
    auto *w1 = new Widget;
    w1->setGeometry(0, 0, width, height);
    layout.place(w1);

    auto const width2 = 130;
    auto const height2 = 100;
    auto *w2 = new Widget;

    w2->setGeometry(0, 0, width2, height2);
    layout.place(w2);

    QCOMPARE(w2->y(), window.height() - span - height - height2);
}

void PopUpLayoutTest::test_MovesWidgetsDownAfterFirstRemoved()
{
    QRect window(0, 0, 1280, 960);
    PopUpLayout layout(window);

    auto const span = 40;

    auto const width = 200;
    auto const height = 110;
    auto *w1 = new Widget;
    w1->setGeometry(0, 0, width, height);
    layout.place(w1);

    auto const width2 = 130;
    auto const height2 = 100;
    auto *w2 = new Widget;

    w2->setGeometry(0, 0, width2, height2);
    layout.place(w2);

    auto *w3 = new Widget;

    w3->setGeometry(0, 0, width2, height2);
    layout.place(w3);

    QCOMPARE(w1->y(), window.height() - span - height);
    QCOMPARE(w2->y(), w1->y() - w2->height());
    QCOMPARE(w3->y(), w2->y() - w3->height());

    layout.remove(w1);

    QCOMPARE(w2->y(), window.height() - span - w2->height());
    QCOMPARE(w3->y(), w2->y() - w3->height());
}

QTEST_MAIN(PopUpLayoutTest)

#include "tst_PopUpLayoutTest.moc"
