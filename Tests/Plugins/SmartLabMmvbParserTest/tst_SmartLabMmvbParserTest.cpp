#include <QtTest>
#include "SmartLabMmvbPlugin/SmartLabMmvbParser.h"
// add necessary includes here

class SmartlabMmvbParserTest : public QObject
{
    Q_OBJECT
    QByteArray arr;
    SmartLabMmvbParser parser;
public:
    SmartlabMmvbParserTest();
    ~SmartlabMmvbParserTest();

private slots:
    void rightRowCount();
    void rightTickers();
    void rightPrices();
    void rightDerivation();
    void rightDerivationWeek();
    void rightDerivationMonth();
    void rightDerivationYear();
};

QByteArray sample(){
    return R"TEST(
           <table class="simple-little-table trades-table" cellspacing="0">
               <tbody><tr>
                   <th style="padding: 4px 10px">№</th>
                   <th><a href="/q/shares/order_by_time/desc/"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/time.svg"></a></th>
                   <th><a href="/q/shares/order_by_short_name/asc/">Название</a></th>
                   <th><a href="/q/shares/order_by_sec_id/asc/">Тикер</a></th>
                   <th class="chartrow"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/blog.svg"></th>
                   <th class="chartrow"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/chart.svg"></th>
                   <th class="chartrow"><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/bars.svg"></th>
                   <th><a href="/q/shares/order_by_last/desc/">Цена,<br> посл</a></th>
                   <th><a href="/q/shares/order_by_last_to_prev_price/desc/">Изм, %</a></th>
                   <th><a class="active" href="/q/shares/order_by_val_to_day/asc/">Объем,<br> млн руб</a></th>
                   <th><a href="/q/shares/order_by_last_to_week_price/desc/">1 нед, %</a></th>
                   <th><a href="/q/shares/order_by_last_to_month_price/desc/">1 м, %</a></th>
                   <th><a href="/q/shares/order_by_last_to_year_price/desc/">ytd, %</a></th>
                   <th><a href="/q/shares/order_by_last_to_12m_price/desc/">12м, %</a></th>
                   <th><a href="/q/shares/order_by_issue_capitalization/desc/">Капит-я<br> млрд руб</a></th>
                   <th><a href="/q/shares/order_by_issue_capitalization_usd/desc/">Капит-я<br> млрд $</a></th>
                   <th><a href="/q/shares/order_by_val_to_day_change/desc/">Изм<br>Объема</a></th>
                   <th><a href="/q/shares/order_by_val_place_change/desc/">Изм поз<br>по Объему</a></th>
                   <th><img class="mart_portf_icon" src="/plugins/trade/templates/skin/default/img/port4.svg"></th>
                   <th><img class="mart_bookmark_icon" src="/plugins/trade/templates/skin/default/img/bookmark_border-24px.svg"></th>
               </tr>

                       <tr class="up">
                   <td>&nbsp;</td>
                   <td>18:51:01</td>
                   <td><a href="/q/index_stocks/IMOEX/">Индекс ММВБ</a></td>
                   <td>IMOEX</td>
                   <td><a class="charticon" href="/gr/MOEX.IMOEX" target="_blank"></a></td>
                   <td><a class="charticon" href="/gr/MOEX.IMOEX" target="_blank"></a></td>
                   <td><a class="charticon2" href="/q/shares_fundamental/" target="_blank"></a></td>
                   <td>3258.31</td>
                   <td>
                                       +1.45%
                               </td>
                   <td>103 055.35</td>
                   <td>+3.0%</td>
                   <td class="up">+8.7%</td>
                   <td class="up">+7.0%</td>
                   <td class="up">+11.1%</td>
                   <td>51 811.52</td>
                   <td>702.89</td>
                   <td>+12%</td>
                   <td>—</td>
                   <td>—</td>
                   <td>—</td>
               </tr>

                       <tr>

                   <td>1</td>
                   <td>18:45:57</td>
                   <td><a href="/forum/SBER">Сбербанк</a></td>
                   <td>SBER</td>
                   <td><a class="charticon" href="/gr/MOEX.SBER" target="_blank"></a></td>
                   <td><a class="charticon" href="/gr/MOEX.SBER" target="_blank"></a></td>
                   <td><a class="charticon2" href="/q/SBER/f/y/" target="_blank"></a></td>
                   <td>281.81</td>
                   <td class="up">
                                       +1.78%
                               </td>
                   <td>19 545.93</td>
                   <td class="up">+7.0%</td>
                   <td class="up">+19.1%</td>
                   <td class="up">+10.6%</td>
                   <td class="up">+18.5%</td>
                   <td>6 334.46</td>
                   <td>85.93</td>
                   <td class="up">+16%</td>
                   <td class="arrow"></td>
                   <td><span class="portfolio_action" type="buy" sec-last="281.81" symbol="MOEX.SBER" title="Добавить Сбербанк в виртуальный портфель">+</span></td>
                   <td><span class="watchlist_action" type="add" symbol="MOEX.SBER" title="Добавить Сбербанк в свой список">+</span></td>
               </tr>
                       <tr>

                   <td>2</td>
                   <td>18:45:34</td>
                   <td><a href="/forum/LKOH">ЛУКОЙЛ</a></td>
                   <td>LKOH</td>
                   <td><a class="charticon" href="/gr/MOEX.SBER" target="_blank"></a></td>
                   <td><a class="charticon" href="/gr/MOEX.LKOH" target="_blank"></a></td>
                   <td><a class="charticon2" href="/q/LKOH/f/y/" target="_blank"></a></td>
                   <td>5315.5</td>
                   <td class="up">
                                       +5.82%
                               </td>
                   <td>11 745.58</td>
                   <td class="up">+5.5%</td>
                   <td class="up">+8.2%</td>
                   <td class="down">-13.8%</td>
                   <td class="down">-12.8%</td>
                   <td>3 682.93</td>
                   <td>49.96</td>
                   <td class="up">+90%</td>
                   <td class="up arrow">+2</td>
                   <td><span class="portfolio_action" type="buy" sec-last="5315.5" symbol="MOEX.LKOH" title="Добавить ЛУКОЙЛ в виртуальный портфель">+</span></td>
                   <td><span class="watchlist_action" type="add" symbol="MOEX.LKOH" title="Добавить ЛУКОЙЛ в свой список">+</span></td>
               </tr>
                       <tr>

                   <td>3</td>
                   <td>18:45:45</td>
                   <td><a href="/forum/GAZP">ГАЗПРОМ ао</a></td>
                   <td>GAZP</td>
                   <td><a class="charticon" href="/gr/MOEX.SBER" target="_blank"></a></td>
                   <td><a class="charticon" href="/gr/MOEX.GAZP" target="_blank"></a></td>
                   <td><a class="charticon2" href="/q/GAZP/f/y/" target="_blank"></a></td>
                   <td>193.3</td>
                   <td class="up">
                                       +2.09%
                               </td>
                   <td>9 523.52</td>
                   <td class="up">+4.1%</td>
                   <td class="up">+12.2%</td>
                   <td class="down">-24.6%</td>
                   <td class="down">-21.7%</td>
                   <td>4 576.09</td>
                   <td>62.08</td>
                   <td class="up">+20%</td>
                   <td class="down arrow">-1</td>
                   <td><span class="portfolio_action" type="buy" sec-last="193.3" symbol="MOEX.GAZP" title="Добавить ГАЗПРОМ ао в виртуальный портфель">+</span></td>
                   <td><span class="watchlist_action" type="add" symbol="MOEX.GAZP" title="Добавить ГАЗПРОМ ао в свой список">+</span></td>
               </tr>
                       <tr>

                   <td>4</td>
                   <td>18:45:47</td>
                   <td><a href="/forum/GMKN">ГМКНорНик</a></td>
                   <td>GMKN</td>
                   <td><a class="charticon" href="/gr/MOEX.SBER" target="_blank"></a></td>
                   <td><a class="charticon" href="/gr/MOEX.GMKN" target="_blank"></a></td>
                   <td><a class="charticon2" href="/q/GMKN/f/y/" target="_blank"></a></td>
                   <td>24050</td>
                   <td class="up">
                                       +3.8%
                               </td>
                   <td>8 729.97</td>
                   <td class="up">+3.9%</td>
                   <td class="up">+17.1%</td>
                   <td class="up">+25.9%</td>
                   <td class="up">+30.2%</td>
                   <td>3 805.80</td>
                   <td>51.63</td>
                   <td class="up">+73%</td>
                   <td class="up arrow">+1</td>
                   <td><span class="portfolio_action" type="buy" sec-last="24050" symbol="MOEX.GMKN" title="Добавить ГМКНорНик в виртуальный портфель">+</span></td>
                   <td><span class="watchlist_action" type="add" symbol="MOEX.GMKN" title="Добавить ГМКНорНик в свой список">+</span></td>
               </tr>
                       <tr>

                   <td>5</td>
                   <td>18:45:02</td>
                   <td><a href="/forum/MGNT">Магнит ао</a></td>
                   <td>MGNT</td>
                   <td><a class="charticon" href="/gr/MOEX.SBER" target="_blank"></a></td>
                   <td><a class="charticon" href="/gr/MOEX.MGNT" target="_blank"></a></td>
                   <td><a class="charticon2" href="/q/MGNT/f/y/" target="_blank"></a></td>
                   <td>5334</td>
                   <td class="down">
                                       -0.93%
                               </td>
                   <td>4 680.99</td>
                   <td class="up">+6.8%</td>
                   <td class="up">+8.4%</td>
                   <td class="up">+55.7%</td>
                   <td class="up">+64.9%</td>
                   <td>543.60</td>
                   <td>7.37</td>
                   <td class="down">-25%</td>
                   <td class="down arrow">-2</td>
                   <td><span class="portfolio_action" type="buy" sec-last="5334" symbol="MOEX.MGNT" title="Добавить Магнит ао в виртуальный портфель">+</span></td>
                   <td><span class="watchlist_action" type="add" symbol="MOEX.MGNT" title="Добавить Магнит ао в свой список">+</span></td>
               </tr>
                   </tbody></table>
           )TEST";
}

SmartlabMmvbParserTest::SmartlabMmvbParserTest()
    : arr(sample())
{
}

SmartlabMmvbParserTest::~SmartlabMmvbParserTest()
{

}

void SmartlabMmvbParserTest::rightRowCount()
{
    std::vector<QByteArray> tickerts = {"SBER", "LKOH", "GAZP", "GMKN", "MGNT"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);
    QCOMPARE(list.size(), tickerts.size());
}

void SmartlabMmvbParserTest::rightTickers()
{
    std::vector<QByteArray> tickerts = {"SBER", "LKOH", "GAZP", "GMKN", "MGNT"};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), tickerts.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].ticker.data(), tickerts[i]);
    }
}

void SmartlabMmvbParserTest::rightPrices()
{
    std::vector<float> prices = {281.81, 5315.5, 193.3, 24050, 5334};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), prices.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].price, prices[i]);
    }
}

void SmartlabMmvbParserTest::rightDerivation()
{
    std::vector<float> derivation = {+1.78, +5.82, +2.09, +3.8, -0.93};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivation, derivation[i]);
    }
}

void SmartlabMmvbParserTest::rightDerivationWeek()
{
    std::vector<float> derivation = {+7, +5.5, +4.1, +3.9, +6.8};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationWeek, derivation[i]);
    }
}

void SmartlabMmvbParserTest::rightDerivationMonth()
{
    std::vector<float> derivation = {+19.1, +8.2, +12.2, +17.1, +8.4};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationMonth, derivation[i]);
    }
}

void SmartlabMmvbParserTest::rightDerivationYear()
{
    std::vector<float> derivation = {+18.5, -12.8, -21.7, +30.2, +64.9};
    StocksList list;
    TimeString time;
    parser.parse(arr, list, time);

    QCOMPARE(list.size(), derivation.size());
    for(std::size_t i = 0; i < list.size(); ++i){
        QCOMPARE(list[i].derivationYear, derivation[i]);
    }
}

QTEST_APPLESS_MAIN(SmartlabMmvbParserTest)

#include "tst_SmartLabMmvbParserTest.moc"
