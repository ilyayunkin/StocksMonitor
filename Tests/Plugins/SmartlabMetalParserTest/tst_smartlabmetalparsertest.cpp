#include <QtTest>
#include "SmartLabMetalPlugin/SmartLabMetalParser.h"
// add necessary includes here

class SmartlabMetalParserTest : public QObject
{
    Q_OBJECT
    QByteArray arr;
    SmartLabMetalParser parser;
public:
    SmartlabMetalParserTest();
    ~SmartlabMetalParserTest();

private slots:
    void rightRowCount();
    void rightTickers();
    void rightPrices();
    void rightDerivation();
};

SmartlabMetalParserTest::SmartlabMetalParserTest()
{
    arr = R"TEST(
              <table id="wrld" class="simple-little-table trades-table" cellspacing="0">
              <tbody><tr>
                  <td colspan="5" style="text-align: center">
                      <h3 style="margin: 3px Auto;">Котировки сырья</h3>
                  </td>
              </tr>
              <tr tkr="FX_IDC.USDBRO" accuracy="2">
                  <td>Нефть Brent</td>
                  <td><a class="charticon" href="/gr/FX_IDC.USDBRO"></a></td>
                  <td>50,40</td>
                  <td class="up">+2,8%</td>
              </tr>
              <tr tkr="TVC.USOIL" accuracy="2">
                  <td>Нефть WTI</td>
                  <td><a class="charticon" href="/gr/TVC.USOIL"></a></td>
                  <td>46,94</td>
                  <td class="up">+2,8%</td>
              </tr>
              <tr tkr="FX_IDC.XAUUSD" accuracy="1">
                  <td>Золото</td>
                  <td><a class="charticon" href="/gr/FX_IDC.XAUUSD"></a></td>
                  <td>1834,2</td>
                  <td class="up down">-0,3%</td>
              </tr>
              <tr tkr="OANDA.XPTUSD" accuracy="1">
                  <td>Платина</td>
                  <td><a class="charticon" href="/gr/OANDA.XPTUSD"></a></td>
                  <td>1022,8</td>
                  <td class="up">+1,9%</td>
              </tr>
              <tr tkr="FX_IDC.XAGUSD" accuracy="1">
                  <td>Серебро</td>
                  <td><a class="charticon" href="/gr/FX_IDC.XAGUSD"></a></td>
                  <td>23,9</td>
                  <td class="up">0,0%</td>
              </tr>
              <tr tkr="TVC.PALLADIUM" accuracy="1">
                  <td>Палладий</td>
                  <td><a class="charticon" href="/gr/TVC.PALLADIUM"></a></td>
                  <td>2340,0</td>
                  <td class="up">+3,0%</td>
              </tr>
              <tr tkr="COMEX.HG1!" accuracy="1">
                  <td>Медь</td>
                  <td><a class="charticon" href="/gr/COMEX.HG1!"></a></td>
                  <td>&nbsp;</td>
                  <td class="up">&nbsp;</td>
              </tr>
              </tbody></table>
          )TEST";
}

SmartlabMetalParserTest::~SmartlabMetalParserTest()
{

}

void SmartlabMetalParserTest::rightRowCount()
{
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);
    QCOMPARE(list.size(), 7u);
}

void SmartlabMetalParserTest::rightTickers()
{
    std::vector<QByteArray> tickerts = {"USDBRO", "USOIL", "XAUUSD", "XPTUSD", "XAGUSD",
                          "PALLADIUM", "HG1!"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), tickerts.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].ticker.data(), tickerts[i]);
    }
}

void SmartlabMetalParserTest::rightPrices()
{
    std::vector<float> prices = {50.4, 46.94, 1834.2, 1022.8, 23.9, 2340.0, 0};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), prices.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].price, prices[i]);
    }
}

void SmartlabMetalParserTest::rightDerivation()
{
    std::vector<float> derivation = {2.8, 2.8, -0.3, +1.9, +0.0, +3.0, 0};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivation, derivation[i]);
    }
}

QTEST_APPLESS_MAIN(SmartlabMetalParserTest)

#include "tst_smartlabmetalparsertest.moc"
