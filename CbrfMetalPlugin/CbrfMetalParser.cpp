#include "CbrfMetalParser.h"

#include <assert.h>

#include <QDateTime>
#include <QDebug>
#include <QDomDocument>

#define WRITE_DEBUG_FILES 0
#define DEBUG_PRINT 0

#if WRITE_DEBUG_FILES
#include <QFile>
#endif

#include "ExceptionClasses.h"

namespace  {
const QString htmlName = "stocks.html";
const QString txtName = "stocks.txt";
const QString tableName = "table.html";
const QLocale locale(QLocale::Russian, QLocale::Russia);
enum
{
    NUM,
    TIME,
    NAME,
    TICKER,
    CHART,
    CHART2,
    PRICE,
    DERIVATION_PC,
    //    VALUE,
    DERIVATION_PC_WEEK,
    DERIVATION_PC_MONTH,
    DERIVATION_PC_YTD,
    DERIVATION_PC_YEAR,
    //    CAPITAL,
    CAPITAL_USD,
    //    VALUE_DERIVATION,
    //    POS_VAL_DER,

    COL_COUNT,
    // Two last columns are icons with site's controls
    COL_COUNT_TOTAL = COL_COUNT + 2
};
}

void CbrfMetalParser::parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                               StocksList &stocks,
                               TimeString &time)
{
    assert(stocks.empty());
//    time = QTime::currentTime().toString().toLatin1().data();

    if(m_DownloadeAwholeDocumentdData.isEmpty())
    {
        throw PageUnavailibleException();
    }

    QDomDocument doc("mydocument");

    if (!doc.setContent(m_DownloadeAwholeDocumentdData)) {
        return;
    }

    QDomElement docElem = doc.documentElement();

    QDomNode currencyNode = docElem.firstChild();
    int i = 1;
    while(!currencyNode.isNull())
    {
        Stock stock;
        QDomElement element = currencyNode.toElement(); // try to convert the node to an element.
        if(!element.isNull())
        {
            QDomNode subNode = element.firstChild();
            auto code = element.attribute("Code");
            time = element.attribute("Date").toStdString();
            if(code == "1"){
                stock.name = "Aurum";
                stock.ticker = "AURUM";
            }else if(code == "2"){
                stock.name = "Silver";
                stock.ticker = "SILVER";
            }if(code == "3"){
                stock.name = "Platinum";
                stock.ticker = "PL";
            }if(code == "4"){
                stock.name = "Palladium";
                stock.ticker = "PALAD";
            }
            while(!subNode.isNull())
            {
                QDomElement subElement = subNode.toElement(); // try to convert the node to an element.
                if(!subElement.isNull())
                {
                    auto tag = subElement.tagName();
                    auto text = subElement.text();

                    if(tag == "Buy")
                    {
                        stock.price = locale.toFloat(text);
                    }
                }
                subNode = subNode.nextSibling();
            }
        }
        stocks.push_back(stock);
        currencyNode = currencyNode.nextSibling();
        ++i;
    }

    if(stocks.empty())
        throw EmptyTableException();

    if(stocks.size() > 4){
        auto newestSubrange = stocks.end() - 4;
        std::rotate(stocks.begin(), newestSubrange, stocks.end());
        stocks.erase(stocks.begin() + 4, stocks.end());
    }
}

