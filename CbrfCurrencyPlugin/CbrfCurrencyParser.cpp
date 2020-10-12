#include "CbrfCurrencyParser.h"

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

void CbrfCurrencyParser::parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                               StocksList &stocks,
                               QByteArray &time)
{
    assert(stocks.empty());
    time = QTime::currentTime().toString().toLatin1();

    if(m_DownloadeAwholeDocumentdData.isEmpty())
    {
        throw PageUnavailibleException();
    }

    QDomDocument doc("mydocument");

    if (!doc.setContent(m_DownloadeAwholeDocumentdData)) {
        return;
    }

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();

    QDomNode currencyNode = docElem.firstChild();
    int i = 1;
    while(!currencyNode.isNull())
    {
        Stock stock;
        stock.rowNum = i;
        QDomElement element = currencyNode.toElement(); // try to convert the node to an element.
        if(!element.isNull())
        {
            QDomNode subNode = element.firstChild();
            while(!subNode.isNull())
            {
                QDomElement subElement = subNode.toElement(); // try to convert the node to an element.
                if(!subElement.isNull())
                {
                    auto tag = subElement.tagName();
                    auto text = subElement.text();

                    if(tag == "Name")
                    {
                        stock.name+= text;
                    }else  if(tag == "Nominal")
                    {
                        stock.name.prepend(text + " ");
                    }else  if(tag == "CharCode")
                    {
                        stock.ticker = text.toLatin1().data();
                    }else  if(tag == "Value")
                    {
                        stock.price = QLocale::system().toFloat(text);
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
}

