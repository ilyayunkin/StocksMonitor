#include <QtTest>
#include "SmartLabMmvbUsaPlugin/SmartLabMmvbUsaParser.h"
// add necessary includes here

class SmartLabMmvbUsaParserTest : public QObject
{
    Q_OBJECT

    QByteArray arr;
    SmartLabMmvbUsaParser parser;
public:
    SmartLabMmvbUsaParserTest();
    ~SmartLabMmvbUsaParserTest();

private slots:
    void rightRowCount();
    void rightTickers();
    void rightPrices();
    void rightDerivation();
    void rightDerivationWeek();
    void rightDerivationMonth();
    void rightDerivationYear();
};

SmartLabMmvbUsaParserTest::SmartLabMmvbUsaParserTest()
{
    arr = R"TEST(
          <table class="simple-little-table trades-table" cellspacing="0">
          <tbody><tr>
          <th style="padding: 4px 10px">№</th>
          <th><a href="/q/usa-stocks-moex/order_by_time/desc/"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/time.svg"></a></th>
          <th><a href="/q/usa-stocks-moex/order_by_short_name/asc/">Название</a></th>
          <th><a href="/q/usa-stocks-moex/order_by_sec_id/asc/">Тикер</a></th>
          <th class="chartrow"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/chart.svg"></th>
          <th class="chartrow"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/bars.svg"></th>
          <th><a href="/q/usa-stocks-moex/order_by_last/desc/">Цена,<br> посл</a></th>
          <th><a href="/q/usa-stocks-moex/order_by_last_to_prev_price/desc/">Изм, %</a></th>
          <th><a class="active" href="/q/usa-stocks-moex/order_by_val_to_day/asc/">Объем,<br> млн руб</a></th>
          <th><a href="/q/usa-stocks-moex/order_by_last_to_week_price/desc/">1 нед, %</a></th>
          <th><a href="/q/usa-stocks-moex/order_by_last_to_month_price/desc/">1 м, %</a></th>
          <th><a href="/q/usa-stocks-moex/order_by_last_to_year_price/desc/">ytd, %</a></th>
          <th><a href="/q/usa-stocks-moex/order_by_last_to_12m_price/desc/">12м, %</a></th>
          <th><a href="/q/usa-stocks-moex/order_by_val_to_day_change/desc/">Изм<br>Объема</a></th>
          <th><a href="/q/usa-stocks-moex/order_by_val_place_change/desc/">Изм поз<br>по Объему</a></th>
          <th><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/port4.svg"></th>
          <th><img class="mart_bookmark_icon" src="/plugins/trade/templates/skin/default/img/bookmark_border-24px.svg"></th>
          </tr>


          <tr>

          <td>1</td>
          <td>18:45:07</td>
          <td>Tesla</td>
          <td>TSLA-RM</td>
          <td><a class="charticon" href="/gr/MOEX.TSLA-RM" target="_blank"></a></td>
          <td></td>
          <td>44799</td>
          <td class="up">
          +0.49%
          </td>
          <td>402.93</td>
          <td class="up">+2.6%</td>
          <td class="up">+47.3%</td>
          <td></td>
          <td></td>
          <td class="up">+32%</td>
          <td class="down arrow">-1</td>
          <td><span class="portfolio_action" type="buy" sec-last="44799" symbol="MOEX.TSLA-RM" title="Добавить Tesla в виртуальный портфель">+</span></td>
          <td><span class="watchlist_action" type="add" symbol="MOEX.TSLA-RM" title="Добавить Tesla в свой список">+</span></td>
          </tr>
          <tr>

          <td>2</td>
          <td>18:40:21</td>
          <td>BABA-адр</td>
          <td>BABA-RM</td>
          <td><a class="charticon" href="/gr/MOEX.BABA-RM" target="_blank"></a></td>
          <td></td>
          <td>19399</td>
          <td class="down">
          -0.29%
          </td>
          <td>81.19</td>
          <td class="down">-2.6%</td>
          <td class="down">-4.8%</td>
          <td></td>
          <td></td>
          <td class="up">+27%</td>
          <td class="up arrow">+4</td>
          <td><span class="portfolio_action" type="buy" sec-last="19399" symbol="MOEX.BABA-RM" title="Добавить BABA-адр в виртуальный портфель">+</span></td>
          <td><span class="watchlist_action" type="add" symbol="MOEX.BABA-RM" title="Добавить BABA-адр в свой список">+</span></td>
          </tr>
          <tr>

          <td>3</td>
          <td>18:42:18</td>
          <td><a href="/forum/BA">Boeing</a></td>
          <td>BA-RM</td>
          <td><a class="charticon" href="/gr/MOEX.BA-RM" target="_blank"></a></td>
          <td></td>
          <td>17146</td>
          <td class="up">
          +0.04%
          </td>
          <td>62.65</td>
          <td class="down">-2.4%</td>
          <td class="up">+19.4%</td>
          <td></td>
          <td></td>
          <td class="up">+94%</td>
          <td class="up arrow">+12</td>
          <td><span class="portfolio_action" type="buy" sec-last="17146" symbol="MOEX.BA-RM" title="Добавить Boeing в виртуальный портфель">+</span></td>
          <td><span class="watchlist_action" type="add" symbol="MOEX.BA-RM" title="Добавить Boeing в свой список">+</span></td>
          </tr>
          <tr>

          <td>4</td>
          <td>18:42:28</td>
          <td><a href="/forum/PFE">Pfizer</a></td>
          <td>PFE-RM</td>
          <td><a class="charticon" href="/gr/MOEX.PFE-RM" target="_blank"></a></td>
          <td></td>
          <td>3060</td>
          <td class="down">
          -1.42%
          </td>
          <td>53.69</td>
          <td class="up">+2.0%</td>
          <td class="up">+0.7%</td>
          <td></td>
          <td></td>
          <td class="down">-80%</td>
          <td class="down arrow">-18</td>
          <td><span class="portfolio_action" type="buy" sec-last="3060" symbol="MOEX.PFE-RM" title="Добавить Pfizer в виртуальный портфель">+</span></td>
          <td><span class="watchlist_action" type="add" symbol="MOEX.PFE-RM" title="Добавить Pfizer в свой список">+</span></td>
          </tr>
          <tr>

          <td>5</td>
          <td>18:45:00</td>
          <td>AmAirlines</td>
          <td>AAL-RM</td>
          <td><a class="charticon" href="/gr/MOEX.AAL-RM" target="_blank"></a></td>
          <td></td>
          <td>1288</td>
          <td class="up">
          +1.74%
          </td>
          <td>48.65</td>
          <td class="up">+8.1%</td>
          <td class="up">+34.2%</td>
          <td></td>
          <td></td>
          <td class="down">-57%</td>
          <td class="down arrow">-9</td>
          <td><span class="portfolio_action" type="buy" sec-last="1288" symbol="MOEX.AAL-RM" title="Добавить AmAirlines в виртуальный портфель">+</span></td>
          <td><span class="watchlist_action" type="add" symbol="MOEX.AAL-RM" title="Добавить AmAirlines в свой список">+</span></td>
          </tr>
          <tr>

          <td>6</td>
          <td>18:45:38</td>
          <td><a href="/forum/NVDA">NVIDIA</a></td>
          <td>NVDA-RM</td>
          <td><a class="charticon" href="/gr/MOEX.NVDA-RM" target="_blank"></a></td>
          <td></td>
          <td>38100</td>
          <td class="down">
          -0.3%
          </td>
          <td>42.03</td>
          <td class="down">-5.0%</td>
          <td class="down">-1.9%</td>
          <td></td>
          <td></td>
          <td class="up">+5%</td>
          <td class="up arrow">+1</td>
          <td><span class="portfolio_action" type="buy" sec-last="38100" symbol="MOEX.NVDA-RM" title="Добавить NVIDIA в виртуальный портфель">+</span></td>
          <td><span class="watchlist_action" type="add" symbol="MOEX.NVDA-RM" title="Добавить NVIDIA в свой список">+</span></td>
          </tr>
          <tr>
          </tbody></table>
          )TEST";
}

SmartLabMmvbUsaParserTest::~SmartLabMmvbUsaParserTest()
{

}

void SmartLabMmvbUsaParserTest::rightRowCount()
{
    std::vector<QByteArray> tickerts = {"TSLA-RM", "BABA-RM", "BA-RM", "PFE-RM", "AAL-RM", "NVDA-RM"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);
    QCOMPARE(list.size(), tickerts.size());
}

void SmartLabMmvbUsaParserTest::rightTickers()
{
    std::vector<QByteArray> tickerts = {"TSLA-RM", "BABA-RM", "BA-RM", "PFE-RM", "AAL-RM", "NVDA-RM"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), tickerts.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].ticker.data(), tickerts[i]);
    }
}

void SmartLabMmvbUsaParserTest::rightPrices()
{
    std::vector<float> prices = {44799, 19399, 17146, 3060, 1288, 38100};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), prices.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].price, prices[i]);
    }
}

void SmartLabMmvbUsaParserTest::rightDerivation()
{
    std::vector<float> derivation = {+0.49, -0.29, +0.04, -1.42, +1.74, -0.3};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivation, derivation[i]);
    }
}

void SmartLabMmvbUsaParserTest::rightDerivationWeek()
{
    std::vector<float> derivation = {+2.6, -2.6, -2.4, +2.0, +8.1, -5.0};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationWeek, derivation[i]);
    }
}

void SmartLabMmvbUsaParserTest::rightDerivationMonth()
{
    std::vector<float> derivation = {+47.3, -4.8, +19.4, +0.7, +34.2, -1.9};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationMonth, derivation[i]);
    }
}

void SmartLabMmvbUsaParserTest::rightDerivationYear()
{
    std::vector<float> derivation = {0, 0, 0, 0, 0, 0};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationYear, derivation[i]);
    }
}

QTEST_APPLESS_MAIN(SmartLabMmvbUsaParserTest)

#include "tst_smartlabmmvbusaparsertest.moc"
