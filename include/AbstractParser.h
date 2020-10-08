#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#include <QByteArray>

#include <memory>

#include "StocksList.h"

class AbstractParser
{
public:
    typedef std::shared_ptr<AbstractParser> Ptr;

    virtual ~AbstractParser(){};
    virtual void parse(const QByteArray &m_DownloadeAwholeDocumentdData,
                      StocksList &stocks,
                      QByteArray &time) = 0;
};

#endif // ABSTRACTPARSER_H
