#ifndef SMARTLABPARSER_H
#define SMARTLABPARSER_H

#include <QByteArray>
#include <QByteArrayList>

#include "StocksList.h"
#include "AbstractParser.h"

class SmartLabParser : public AbstractParser
{
    static QByteArray getDiv(const QByteArray &wholeDocument);
    static QByteArray getTable(const QByteArray &div);
    static QByteArrayList getRows(const QByteArray &table);
    static QByteArrayList getCols(const QByteArray &table);
    static QByteArray getA(const QByteArray &tableCol);
    static float getPercentage(const QByteArray &tableCol);
public:
    void parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                      StocksList &stocks,
                      QByteArray &time) override;
};

#endif // SMARTLABPARSER_H
