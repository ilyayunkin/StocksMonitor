#include "stocksmonitor.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

#include <algorithm>

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

    COL_COUNT
};
}


StocksMonitor::StocksMonitor(QObject *parent) : QObject(parent)
{
    requestWordsFromTheInternet();
}


void StocksMonitor::requestWordsFromTheInternet()
{
    qDebug() << __PRETTY_FUNCTION__;
    QNetworkRequest request(QUrl("https://smart-lab.ru/q/shares/order_by_last_to_prev_price/asc/"));
    m_WebCtrl.get(request);
    connect(&m_WebCtrl, &QNetworkAccessManager::finished,
            this, &StocksMonitor::fileDownloaded);
}

void StocksMonitor::fileDownloaded(QNetworkReply *r)
{
    const QByteArray m_DownloadeAwholeDocumentdData = r->readAll();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "received" << m_DownloadeAwholeDocumentdData.size();
    //emit a signal
    r->deleteLater();

    QDateTime begin = QDateTime::currentDateTime();
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
    QDateTime t1 = QDateTime::currentDateTime();

    QByteArrayList tableRows;
    {
        QByteArray div = getDiv(m_DownloadeAwholeDocumentdData);
        QByteArray table = getTable(div);
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
        tableRows = getRows(table);
    }
    QDateTime t2 = QDateTime::currentDateTime();
    StocksList stocks;
    for(auto &row : tableRows)
    {
        QByteArrayList tableCols = getCols(row);
        if(tableCols.size() >= COL_COUNT)
        {
            qDebug() << __PRETTY_FUNCTION__;
            qDebug() << tableCols;
            bool b = false;
            int rowNum = QString(tableCols[NUM]).toInt(&b);
            if(b)
            {
                Stock stock;
                qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "rowNum" << rowNum;
                stock.rowNum = rowNum;
                stock.name = QString(getA(tableCols.at(NAME)));
                stock.ticker = tableCols.at(TICKER);
                stock.price = QString(tableCols.at(PRICE)).toDouble();
                stocks.push_back(stock);
            }
        }
    }
    QDateTime t3 = QDateTime::currentDateTime();
    qDebug() << __PRETTY_FUNCTION__
             << "Timings: " << begin.secsTo(t1)
             << t1.secsTo(t2)
             << t2.secsTo(t3);
    qDebug() << __PRETTY_FUNCTION__
             << "Stocks count: " << stocks.size();
    emit downloaded();
}

QByteArray StocksMonitor::getDiv(const QByteArray &wholeDocument)
{
    constexpr char divBegin[] = "<div style=\"display: inline-block\">";
    constexpr char divEnd[] = "</div>";

    const int fieldBegin = wholeDocument.indexOf(divBegin);
    assert(fieldBegin != -1);
    if(fieldBegin == -1)
    {
    }

    const int fieldEnd = wholeDocument.indexOf(divEnd, fieldBegin);
    assert(fieldEnd != -1);
    if(fieldEnd == -1)
    {
    }

    QByteArray paragraph = wholeDocument.mid(fieldBegin, fieldEnd - fieldBegin);
    paragraph.replace("\t", "");

    return paragraph;
}

QByteArray StocksMonitor::getTable(const QByteArray &div)
{
    constexpr char divBegin[] = "<table class=\"simple-little-table trades-table\" cellspacing=\"0\">";
    constexpr char divEnd[] = "</table>";

    const int fieldBegin = div.indexOf(divBegin);
    assert(fieldBegin != -1);
    if(fieldBegin == -1)
    {
    }

    const int fieldEnd = div.indexOf(divEnd, fieldBegin);
    assert(fieldEnd != -1);
    if(fieldEnd == -1)
    {
    }

    QByteArray paragraph = div.mid(fieldBegin, fieldEnd - fieldBegin);
    paragraph.replace("\t", "");

    return paragraph;

}

QByteArrayList StocksMonitor::getRows(const QByteArray &table)
{
    QByteArrayList ret;
    constexpr char divBegin[] = "<tr>";
    constexpr char divEnd[] = "</tr>";
    int beginPos = 0;
    int i = 0;

    while((beginPos = table.indexOf(divBegin, beginPos)) != -1)
    {
        const int fieldBegin = beginPos;
        //assert(fieldBegin != -1);
        if(fieldBegin == -1)
        {
        }

        const int fieldEnd = beginPos = table.indexOf(divEnd, fieldBegin);
        //assert(fieldEnd != -1);
        if(fieldEnd == -1)
        {
        }

        QByteArray tableRow = table.mid(fieldBegin, fieldEnd - fieldBegin);
        tableRow.replace("\t", "");
        ret.push_back(tableRow);
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
        ++i;
        assert(i < 500);
    }

    return ret;
}

QByteArrayList StocksMonitor::getCols(const QByteArray &table)
{
    QByteArrayList ret;
    constexpr char divBegin[] = "<td";
    constexpr char divEnd[] = "</td>";
    int beginPos = 0;
    int i = 0;

    while((beginPos = table.indexOf(divBegin, beginPos)) != -1)
    {
        int fieldBegin = table.indexOf(">", beginPos);
        //assert(fieldBegin != -1);
        if(fieldBegin == -1)
        {
        }
        ++fieldBegin;

        const int fieldEnd = beginPos = table.indexOf(divEnd, fieldBegin);
        //assert(fieldEnd != -1);
        if(fieldEnd == -1)
        {
        }

        QByteArray tableCol = table.mid(fieldBegin, fieldEnd - fieldBegin);
        tableCol.replace("\t", "");
        ret.push_back(tableCol);

        ++i;
        assert(i < 500);
    }

    return ret;
}

QByteArray StocksMonitor::getA(const QByteArray &tableCol)
{
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << tableCol;

    QByteArray ret = tableCol;
    constexpr char divBegin[] = "<a";
    constexpr char divEnd[] = "</a>";

    const int beginPos = tableCol.indexOf(divBegin);
    // There could be unformated text like <td>ИСУ ГК-3</td>
    //    assert(beginPos != -1);
    if(beginPos == -1)
    {
        return ret;
    }

    int fieldBegin = tableCol.indexOf(">", beginPos);
    //    assert(fieldBegin != -1);
    if(fieldBegin == -1)
    {
        return ret;
    }
    ++fieldBegin;

    const int fieldEnd = tableCol.indexOf(divEnd, fieldBegin);
    //assert(fieldEnd != -1);
    if(fieldEnd == -1)
    {
        return ret;
    }

    ret = tableCol.mid(fieldBegin, fieldEnd - fieldBegin);

    return ret;
}
