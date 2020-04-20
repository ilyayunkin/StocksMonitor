#ifndef SMARTLABPARSER_H
#define SMARTLABPARSER_H

#include <QByteArray>
#include <QByteArrayList>

#include "StocksList.h"

class SmartLabParser
{
    static QByteArray getDiv(const QByteArray &wholeDocument);
    static QByteArray getTable(const QByteArray &div);
    static QByteArrayList getRows(const QByteArray &table);
    static QByteArrayList getCols(const QByteArray &table);
    static QByteArray getA(const QByteArray &tableCol);
    static float getPercentage(const QByteArray &tableCol);
public:
    static void parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                      StocksList &stocks,
                      QByteArray &time);
};

#endif // SMARTLABPARSER_H
