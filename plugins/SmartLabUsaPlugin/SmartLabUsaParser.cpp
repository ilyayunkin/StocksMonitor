#include "SmartLabUsaParser.h"

#include <assert.h>
#include <tuple>

#include <QDateTime>
#include <QDebug>

#define WRITE_DEBUG_FILES 0
#define DEBUG_PRINT 0

#if WRITE_DEBUG_FILES
#include <QFile>
#endif

#include "ExceptionClasses.h"
#include <QByteArrayList>

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

QByteArray getTable(const QByteArray &div)
{
    constexpr char divBegin[] = "<table";
    constexpr char divEnd[] = "</table>";

    const int fieldBegin = div.indexOf(divBegin);
    if(fieldBegin == -1)
    {
        throw NoTableException();
    }

    const int fieldEnd = div.indexOf(divEnd, fieldBegin);
    if(fieldEnd == -1)
    {
        throw NoTableException();
    }

    QByteArray paragraph = div.mid(fieldBegin, fieldEnd - fieldBegin);

    return paragraph;

}

#if WRITE_DEBUG_FILES
void writeDebugFile(const QString &filename, QByteArray data)
{
    QFile f(filename);
    f.open(QIODevice::WriteOnly);
    if(f.isOpen())
    {
        auto count = f.write(data);
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "written" << count << f.size();
        f.flush();
        f.close();
    }
}
#endif

QByteArrayList getRows(const QByteArray &table)
{
    QByteArrayList ret;
    constexpr char divBegin[] = "<tr";
    constexpr char divEnd[] = "</tr>";
    int beginPos = 0;
    int i = 0;

    while((beginPos = table.indexOf(divBegin, beginPos)) != -1)
    {
        const int fieldBegin = beginPos;
        if(fieldBegin == -1)
        {
        }

        const int fieldEnd = beginPos = table.indexOf(divEnd, fieldBegin);
        if(fieldEnd == -1)
        {
        }

        QByteArray tableRow = table.mid(fieldBegin, fieldEnd - fieldBegin);
        ret.push_back(tableRow);
#if WRITE_DEBUG_FILES
        writeDebugFile(QString::number(i) +".txt", tableRow);
#endif
        ++i;
    }

    return ret;
}

QByteArrayList getCols(const QByteArray &table)
{
    QByteArrayList ret;
    constexpr char divBegin[] = "<td";
    constexpr char divEnd[] = "</td>";
    int beginPos = 0;
    int i = 0;

    while((beginPos = table.indexOf(divBegin, beginPos)) != -1)
    {
        int fieldBegin = table.indexOf(">", beginPos);
        if(fieldBegin == -1)
        {
        }
        ++fieldBegin;

        const int fieldEnd = beginPos = table.indexOf(divEnd, fieldBegin);
        if(fieldEnd == -1)
        {
        }

        QByteArray tableCol = table.mid(fieldBegin, fieldEnd - fieldBegin);
        ret.push_back(tableCol);

        ++i;
        if(i > COL_COUNT_TOTAL)
        {
            throw TooManyColsException();
        }
    }

    return ret;
}

typedef std::tuple<StockName, Url> NameUrlTuple;
NameUrlTuple getA(const QByteArray &tableCol)
{
    Url url;
    constexpr char divBegin[] = "<a";
    constexpr char divEnd[] = "</a>";

    const int beginPos = tableCol.indexOf(divBegin);
    // There could be unformated text like <td>ИСУ ГК-3</td>
    if(beginPos == -1)
    {
        return NameUrlTuple();
    }
    {
        const QByteArray hrefBegin = "href=\"";
        const int hrefPos = tableCol.indexOf(hrefBegin, beginPos);
        if(hrefPos !=-1)
        {
            const int quoteClosePos = tableCol.indexOf("\"", hrefPos + hrefBegin.length());
            if(quoteClosePos != -1)
            {
                auto urlTmp = tableCol.mid(hrefPos + hrefBegin.length(), quoteClosePos - hrefPos - hrefBegin.length());
                if(!urlTmp.isEmpty())
                {
                    urlTmp.prepend("https://smart-lab.ru");
                    url = urlTmp.data();
                }
            }
        }
    }

    int fieldBegin = tableCol.indexOf(">", beginPos);
    if(fieldBegin == -1)
    {
        return NameUrlTuple();
    }
    ++fieldBegin;

    const int fieldEnd = tableCol.indexOf(divEnd, fieldBegin);
    if(fieldEnd == -1)
    {
        return NameUrlTuple();
    }

    return NameUrlTuple(tableCol.mid(fieldBegin, fieldEnd - fieldBegin)
                        , url);
}

float getPercentage(const QByteArray &tableCol)
{
    return QString(tableCol).replace('%', ' ').toDouble();
}

}

void SmartLabUsaParser::parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                              StocksList &stocks,
                              TimeString &time)
{
    assert(stocks.empty());
    time = QTime::currentTime().toString().toLatin1().data();

    if(m_DownloadeAwholeDocumentdData.isEmpty())
        throw PageUnavailibleException();

    QDateTime begin = QDateTime::currentDateTime();
#if WRITE_DEBUG_FILES
        writeDebugFile(htmlName, m_DownloadeAwholeDocumentdData);
#endif
#if DEBUG_PRINT
    QDateTime t1 = QDateTime::currentDateTime();
#endif
    QByteArrayList tableRows = [&]
    {
        QByteArray table = getTable(m_DownloadeAwholeDocumentdData);
#if WRITE_DEBUG_FILES
        writeDebugFile(tableName, table);
#endif
        return getRows(table);
    }();

    stocks.reserve(tableRows.size());
#if DEBUG_PRINT
    QDateTime t2 = QDateTime::currentDateTime();
#endif
    {
        int i = 0;
        for(auto &row : tableRows)
        {
            QByteArrayList tableCols = getCols(row);
            if(tableCols.size() >= COL_COUNT)
            {
                bool b = false;
                int rowNum = QString(tableCols[NUM]).toInt(&b);
                (void)rowNum;
                if(b)
                {
                    QString name;
                    Url url;
                    std::tie(name, url) = getA(tableCols.at(NAME));
                    stocks.push_back(Stock(name,
                                           tableCols.at(TICKER).data(),
                                           url,
                                           QString(tableCols.at(PRICE)).toDouble(),
                                           getPercentage(tableCols.at(DERIVATION_PC)),
                                           getPercentage(tableCols.at(DERIVATION_PC_WEEK)),
                                           getPercentage(tableCols.at(DERIVATION_PC_MONTH)),
                                           getPercentage(tableCols.at(DERIVATION_PC_YEAR))));
                }
            }
            ++i;
        }
    }
#if DEBUG_PRINT
    QDateTime t3 = QDateTime::currentDateTime();
    qDebug() << __PRETTY_FUNCTION__
             << "Timings: " << begin.msecsTo(t1)
             << t1.msecsTo(t2)
             << t2.msecsTo(t3);
#endif

    if(stocks.empty())
        throw EmptyTableException();
}
