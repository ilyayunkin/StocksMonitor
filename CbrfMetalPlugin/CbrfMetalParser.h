#ifndef SMARTLABPARSER_H
#define SMARTLABPARSER_H

#include <QByteArray>

#include "StocksList.h"
#include "AbstractParser.h"

class CbrfMetalParser : public AbstractParser
{
public:
    void parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                      StocksList &stocks,
                      TimeString &time) override;
};

#endif // SMARTLABPARSER_H
