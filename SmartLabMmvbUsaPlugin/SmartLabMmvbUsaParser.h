#ifndef SMARTLABPARSER_H
#define SMARTLABPARSER_H

#include <QByteArray>

#include "StocksList.h"
#include "AbstractParser.h"

class SmartLabMmvbUsaParser : public AbstractParser
{
public:
    void parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                      StocksList &stocks,
                      std::string &time) override;
};

#endif // SMARTLABPARSER_H
