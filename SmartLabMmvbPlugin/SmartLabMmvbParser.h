#ifndef SMARTLABPARSER_H
#define SMARTLABPARSER_H

#include <QByteArray>
#include <QByteArrayList>

#include "StocksList.h"
#include "AbstractParser.h"

class SmartLabMmvbParser : public AbstractParser
{
public:
    void parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                      StocksList &stocks,
                      QByteArray &time) override;
};

#endif // SMARTLABPARSER_H
