#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#include <QByteArray>

#include <memory>
#include <string>

#include "StocksList.h"

class AbstractParser
{
public:
    typedef std::unique_ptr<AbstractParser> Ptr;

    virtual ~AbstractParser(){};
    virtual void parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                      StocksList &stocks,
                      std::string &time) = 0;
};

#endif // ABSTRACTPARSER_H
