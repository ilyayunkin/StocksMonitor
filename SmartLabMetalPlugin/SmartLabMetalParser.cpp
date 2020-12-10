#include "SmartLabMetalParser.h"

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
    NAME,
    CHART,
    PRICE,
    DERIVATION_PC,
    COL_COUNT,
    // Two last columns are icons with site's controls
    COL_COUNT_TOTAL = COL_COUNT + 2
};

QByteArray getTable(const QByteArray &div)
{
    constexpr char divBegin[] = "<table id=\"wrld\" class=\"simple-little-table trades-table\"";
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

QByteArray getTicker(const QByteArray &row)
{
    const auto beginPos = row.indexOf('.');
    if(beginPos != -1){
        const auto endPos = row.indexOf('\"', beginPos);
        if(endPos != -1){
            return row.mid(beginPos + 1, endPos - beginPos - 1);
        }
    }
    return "UNKNOWN";
}

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
        {
            QFile f(QString::number(i) +".txt");
            f.open(QIODevice::WriteOnly);
            if(f.isOpen())
            {
                auto count = f.write(tableRow);
                qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "written" << count << f.size();
                f.flush();
                f.close();
            }
        }
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

Url getUrl(const QByteArray &tableCol)
{
    Url url;
    constexpr char divBegin[] = "<a";
    constexpr char divEnd[] = "</a>";

    const int beginPos = tableCol.indexOf(divBegin);
    // There could be unformated text like <td>ИСУ ГК-3</td>
    if(beginPos == -1)
    {
        return Url();
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
        return Url();
    }
    ++fieldBegin;

    const int fieldEnd = tableCol.indexOf(divEnd, fieldBegin);
    if(fieldEnd == -1)
    {
        return Url();
    }

    return url;
}

float getPercentage(const QByteArray &tableCol)
{
    return QString(tableCol).replace('%', ' ').replace(',','.').toDouble();
}

float getPrice(const QByteArray &tableCol)
{
    return QString(tableCol).replace(',', '.').toDouble();
}

}

void SmartLabMetalParser::parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                                StocksList &stocks,
                                TimeString &time)
{
    assert(stocks.empty());
    time = QTime::currentTime().toString().toLatin1().data();

    if(m_DownloadeAwholeDocumentdData.isEmpty())
        throw PageUnavailibleException();

    QDateTime begin = QDateTime::currentDateTime();
#if WRITE_DEBUG_FILES
    {
        QFile f(htmlName);
        f.open(QIODevice::WriteOnly);
        if(f.isOpen())
        {
            auto count = f.write(m_DownloadeAwholeDocumentdData);
            qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "written" << count << f.size();
            f.flush();
            f.close();
        }
    }
#endif
#if DEBUG_PRINT
    QDateTime t1 = QDateTime::currentDateTime();
#endif
    QByteArray table = getTable(m_DownloadeAwholeDocumentdData);
#if WRITE_DEBUG_FILES
    {
        QFile f(tableName);
        f.open(QIODevice::WriteOnly);
        if(f.isOpen())
        {
            auto count = f.write(table);
            qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "written" << count << f.size();
            f.flush();
            f.close();
        }
    }
#endif
    QByteArrayList tableRows = getRows(table);

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
                if(i > 0)
                {
                    Url url = getUrl(tableCols.at(CHART));
                    stocks.push_back(Stock(tableCols.at(NAME),
                                           getTicker(row).data(),
                                           url,
                                           getPrice(tableCols.at(PRICE)),
                                           getPercentage(tableCols.at(DERIVATION_PC))));
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
