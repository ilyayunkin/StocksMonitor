#include <QtTest>
#include "SmartLabUsaPlugin/SmartLabUsaParser.h"

// add necessary includes here

class SmartLabUsaParserTest : public QObject
{
    Q_OBJECT

    QByteArray arr;
    SmartLabUsaParser parser;
public:
    SmartLabUsaParserTest();
    ~SmartLabUsaParserTest();

private slots:
    void rightRowCount();
    void rightTickers();
    void rightPrices();
    void rightDerivation();
    void rightDerivationWeek();
    void rightDerivationMonth();
    void rightDerivationYear();
};

SmartLabUsaParserTest::SmartLabUsaParserTest()
{
    arr = R"TEST(
          <table id="usa_shares" class="simple-little-table trades-table" cellspacing="0">
              <tbody><tr>
                  <th style="padding: 4px 10px">№</th>
                  <th><a href="/q/usa/order_by_time/desc/"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/time.svg"></a></th>
                  <th><a class="active" href="/q/usa/order_by_short_name/desc/">Название</a></th>
                  <th><a href="/q/usa/order_by_sec_id/asc/">Тикер</a></th>
                  <th class="chartrow"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/chart.svg"></th>
                  <th class="chartrow"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/bars.svg"></th>
                  <th><a href="/q/usa/order_by_last/desc/">Цена<br> посл, $</a></th>
                  <th><a href="/q/usa/order_by_last_to_prev_price/desc/">Изм, %</a></th>
                  <th><a href="/q/usa/order_by_last_to_week_price/desc/">1 нед, %</a></th>
                  <th><a href="/q/usa/order_by_last_to_month_price/desc/">1 м, %</a></th>
                  <th><a href="/q/usa/order_by_last_to_year_price/desc/">ytd, %</a></th>
                  <th><a href="/q/usa/order_by_last_to_12m_price/desc/">12м, %</a></th>
                  <th><a href="/q/usa/order_by_issue_capitalization_usd/desc/">Капит-я<br> млрд $</a></th>
                  <th><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/port4.svg"></th>
                  <th><img class="mart_bookmark_icon" src="/plugins/trade/templates/skin/default/img/bookmark_border-24px.svg"></th>
              </tr>

                      <tr>

                  <td>1</td>
                  <td>03:59:00 pm</td>
                  <td><a href="/forum/FLWS">1-800-FLOWERS</a></td>
                  <td>FLWS</td>
                  <td><a class="charticon" href="/gr/NASDAQ.FLWS" target="_blank"></a></td>
                  <td></td>
                  <td>22.8</td>
                  <td class="down">
                                      -2.12%
                              </td>
                  <td class="down">-2.1%</td>
                  <td class="up">+16.7%</td>
                  <td></td>
                  <td></td>
                  <td></td>
                  <td><span class="portfolio_action" type="buy" sec-last="22.8" symbol="NASDAQ.FLWS" title="Добавить 1-800-FLOWERS в виртуальный портфель">+</span></td>
                  <td><span class="watchlist_action" type="add" symbol="NASDAQ.FLWS" title="Добавить 1-800-FLOWERS в свой список">+</span></td>
              </tr>
                      <tr>

                  <td>2</td>
                  <td>03:59:00 pm</td>
                  <td><a href="/forum/DDD">3D Systems Corporation</a></td>
                  <td>DDD</td>
                  <td><a class="charticon" href="/gr/NYSE.DDD" target="_blank"></a></td>
                  <td></td>
                  <td>10.335</td>
                  <td class="up">
                                      +1.27%
                              </td>
                  <td class="up">+2.0%</td>
                  <td class="up">+41.3%</td>
                  <td></td>
                  <td></td>
                  <td></td>
                  <td><span class="portfolio_action" type="buy" sec-last="10.335" symbol="NYSE.DDD" title="Добавить 3D Systems Corporation в виртуальный портфель">+</span></td>
                  <td><span class="watchlist_action" type="add" symbol="NYSE.DDD" title="Добавить 3D Systems Corporation в свой список">+</span></td>
              </tr>
                      <tr>

                  <td>3</td>
                  <td>03:59:00 pm</td>
                  <td><a href="/forum/MMM">3M Company</a></td>
                  <td>MMM</td>
                  <td><a class="charticon" href="/gr/NYSE.MMM" target="_blank"></a></td>
                  <td></td>
                  <td>173.495</td>
                  <td class="up">
                                      +1.97%
                              </td>
                  <td class="up">+1.0%</td>
                  <td class="up">+2.6%</td>
                  <td></td>
                  <td class="up">+3.5%</td>
                  <td></td>
                  <td><span class="portfolio_action" type="buy" sec-last="173.495" symbol="NYSE.MMM" title="Добавить 3M Company в виртуальный портфель">+</span></td>
                  <td><span class="watchlist_action" type="add" symbol="NYSE.MMM" title="Добавить 3M Company в свой список">+</span></td>
              </tr>
                      <tr>

                  <td>4</td>
                  <td>03:59:00 pm</td>
                  <td><a href="/forum/AOS">A. O. Smith</a></td>
                  <td>AOS</td>
                  <td><a class="charticon" href="/gr/NYSE.AOS" target="_blank"></a></td>
                  <td></td>
                  <td>56.305</td>
                  <td class="up">
                                      +1.84%
                              </td>
                  <td class="up">+2.5%</td>
                  <td class="up">+2.9%</td>
                  <td></td>
                  <td></td>
                  <td></td>
                  <td><span class="portfolio_action" type="buy" sec-last="56.305" symbol="NYSE.AOS" title="Добавить A. O. Smith в виртуальный портфель">+</span></td>
                  <td><span class="watchlist_action" type="add" symbol="NYSE.AOS" title="Добавить A. O. Smith в свой список">+</span></td>
              </tr>
                      <tr>

                  <td>5</td>
                  <td>03:59:00 pm</td>
                  <td><a href="/forum/AIR">AAR</a></td>
                  <td>AIR</td>
                  <td><a class="charticon" href="/gr/NYSE.AIR" target="_blank"></a></td>
                  <td></td>
                  <td>34.04</td>
                  <td class="down">
                                      -0.03%
                              </td>
                  <td class="up">+2.5%</td>
                  <td class="up">+38.0%</td>
                  <td></td>
                  <td class="down">-24.2%</td>
                  <td></td>
                  <td><span class="portfolio_action" type="buy" sec-last="34.04" symbol="NYSE.AIR" title="Добавить AAR в виртуальный портфель">+</span></td>
                  <td><span class="watchlist_action" type="add" symbol="NYSE.AIR" title="Добавить AAR в свой список">+</span></td>
              </tr>
                      <tr>
                  </tbody></table>
            )TEST";
}

SmartLabUsaParserTest::~SmartLabUsaParserTest()
{

}

void SmartLabUsaParserTest::rightRowCount()
{
    std::vector<QByteArray> tickerts = {"FLWS", "DDD", "MMM", "AOS", "AIR"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);
    QCOMPARE(list.size(), tickerts.size());
}

void SmartLabUsaParserTest::rightTickers()
{
    std::vector<QByteArray> tickerts = {"FLWS", "DDD", "MMM", "AOS", "AIR"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), tickerts.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].ticker.data(), tickerts[i]);
    }
}

void SmartLabUsaParserTest::rightPrices()
{
    std::vector<float> prices = {22.8, 10.335, 173.495, 56.305, 34.04};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), prices.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].price, prices[i]);
    }
}

void SmartLabUsaParserTest::rightDerivation()
{
    std::vector<float> derivation = {-2.12, +1.27, +1.97, +1.84, -0.03};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivation, derivation[i]);
    }
}

void SmartLabUsaParserTest::rightDerivationWeek()
{
    std::vector<float> derivation = {-2.1, +2.0, +1.0, +2.5, +2.5};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationWeek, derivation[i]);
    }
}

void SmartLabUsaParserTest::rightDerivationMonth()
{
    std::vector<float> derivation = {+16.7, +41.3, +2.6, +2.9, +38.0};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationMonth, derivation[i]);
    }
}

void SmartLabUsaParserTest::rightDerivationYear()
{
    std::vector<float> derivation = {0, 0, +3.5, 0, -24.2};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationYear, derivation[i]);
    }
}
QTEST_APPLESS_MAIN(SmartLabUsaParserTest)

#include "tst_smartlabusaparsertest.moc"
