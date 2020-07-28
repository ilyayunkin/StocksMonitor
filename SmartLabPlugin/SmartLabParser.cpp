#include "SmartLabParser.h"

#include <assert.h>

#include <QDateTime>
#include <QDebug>

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
    VALUE,
    DERIVATION_PC_WEEK,
    DERIVATION_PC_MONTH,
    DERIVATION_PC_YTD,
    DERIVATION_PC_YEAR,
    CAPITAL,
    CAPITAL_USD,
    VALUE_DERIVATION,
    POS_VAL_DER,

    COL_COUNT,
    // Two last columns are icons with site's controls
    COL_COUNT_TOTAL = COL_COUNT + 2
};
}

void SmartLabParser::parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                          StocksList &stocks,
                          QByteArray &time)
{
    assert(stocks.empty());

    if(m_DownloadeAwholeDocumentdData.isEmpty())
    {
        throw PageUnavailibleException();
    }

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
    QDateTime t1 = QDateTime::currentDateTime();

    QByteArrayList tableRows;
    {
        QByteArray div = getDiv(m_DownloadeAwholeDocumentdData);
        QByteArray table = getTable(div);
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
        tableRows = getRows(table);
    }
    QDateTime t2 = QDateTime::currentDateTime();
    {
        int i = 0;
        for(auto &row : tableRows)
        {
            QByteArrayList tableCols = getCols(row);
            if(tableCols.size() >= COL_COUNT)
            {
                bool b = false;
                int rowNum = QString(tableCols[NUM]).toInt(&b);
                if(b)
                {
                    Stock stock;
                    stock.rowNum = rowNum;
                    stock.name = QString(getA(tableCols.at(NAME)));
                    stock.ticker = tableCols.at(TICKER);
                    stock.price = QString(tableCols.at(PRICE)).toDouble();
                    stock.derivation = getPercentage(tableCols.at(DERIVATION_PC));
                    stock.derivationWeek = getPercentage(tableCols.at(DERIVATION_PC_WEEK));
                    stock.derivationMonth = getPercentage(tableCols.at(DERIVATION_PC_MONTH));
                    stock.derivationYear = getPercentage(tableCols.at(DERIVATION_PC_YEAR));

                    stocks.push_back(stock);
                }else if(i == 1)
                {
                    QByteArray t = tableCols.at(TIME);
                    time = t;
                }
            }
            ++i;
        }
    }
    QDateTime t3 = QDateTime::currentDateTime();
#if DEBUG_PRINT
    qDebug() << __PRETTY_FUNCTION__
             << "Timings: " << begin.secsTo(t1)
             << t1.secsTo(t2)
             << t2.secsTo(t3);
    qDebug() << __PRETTY_FUNCTION__
             << "Stocks count: " << stocks.size();
#endif

    if(stocks.empty())
        throw EmptyTableException();
}

QByteArray SmartLabParser::getDiv(const QByteArray &wholeDocument)
{
    constexpr char divBegin[] = "<div style=\"display: inline-block\">";
    constexpr char divEnd[] = "</div>";

    const int fieldBegin = wholeDocument.indexOf(divBegin);
    if(fieldBegin == -1)
    {
        throw NoDivisionException();
    }

    const int fieldEnd = wholeDocument.indexOf(divEnd, fieldBegin);
    if(fieldEnd == -1)
    {
        throw NoDivisionException();
    }

    QByteArray paragraph = wholeDocument.mid(fieldBegin);
    paragraph.replace("\t", "");
    paragraph.replace("\r", "");
    paragraph.replace("\n", "");

    return paragraph;
}

QByteArray SmartLabParser::getTable(const QByteArray &div)
{
    constexpr char divBegin[] = "<table class=\"simple-little-table trades-table\" cellspacing=\"0\">";
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

QByteArrayList SmartLabParser::getRows(const QByteArray &table)
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
        assert(i < 500);
    }

    return ret;
}

QByteArrayList SmartLabParser::getCols(const QByteArray &table)
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

QByteArray SmartLabParser::getA(const QByteArray &tableCol)
{
    QByteArray ret = tableCol;
    constexpr char divBegin[] = "<a";
    constexpr char divEnd[] = "</a>";

    const int beginPos = tableCol.indexOf(divBegin);
    // There could be unformated text like <td>ИСУ ГК-3</td>
    if(beginPos == -1)
    {
        return ret;
    }

    int fieldBegin = tableCol.indexOf(">", beginPos);
    if(fieldBegin == -1)
    {
        return ret;
    }
    ++fieldBegin;

    const int fieldEnd = tableCol.indexOf(divEnd, fieldBegin);
    if(fieldEnd == -1)
    {
        return ret;
    }

    ret = tableCol.mid(fieldBegin, fieldEnd - fieldBegin);

    return ret;
}

float SmartLabParser::getPercentage(const QByteArray &tableCol)
{
    return QString(tableCol).replace('%', ' ').toDouble();
}


