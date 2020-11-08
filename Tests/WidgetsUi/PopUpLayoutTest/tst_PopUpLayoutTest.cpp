#include <QtTest>
#include <QCoreApplication>
#include <QWidget>
#include <QDesktopWidget>

#include "WidgetsUi/PopUp/PopUpLayout.h"
// add necessary includes here

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
    PopUpLayout layout;
    QRect window = QApplication::desktop()->availableGeometry();

    auto const span = 40;
    auto const width = 200;
    auto const height = 100;
    QWidget *w = new QWidget;
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
    PopUpLayout layout;
    QRect window = QApplication::desktop()->availableGeometry();

    auto const span = 40;

    auto const width = 200;
    auto const height = 110;
    QWidget *w1 = new QWidget;
    w1->setGeometry(0, 0, width, height);
    layout.place(w1);

    auto const width2 = 130;
    auto const height2 = 100;
    QWidget *w2 = new QWidget;

    w2->setGeometry(0, 0, width2, height2);
    layout.place(w2);

    QCOMPARE(w2->y(), window.height() - span - height - height2);
}

void PopUpLayoutTest::test_MovesWidgetsDownAfterFirstRemoved()
{
    PopUpLayout layout;
    QRect window = QApplication::desktop()->availableGeometry();

    auto const span = 40;

    auto const width = 200;
    auto const height = 110;
    QWidget *w1 = new QWidget;
    w1->setGeometry(0, 0, width, height);
    layout.place(w1);

    auto const width2 = 130;
    auto const height2 = 100;
    QWidget *w2 = new QWidget;

    w2->setGeometry(0, 0, width2, height2);
    layout.place(w2);

    QWidget *w3 = new QWidget;

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
