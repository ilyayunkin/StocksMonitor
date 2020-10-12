#ifndef SMARTLABPLUGIN_H
#define SMARTLABPLUGIN_H

#include "SourcePluginInterface.h"
#include "CbrfCurrencyParser.h"

class CbrfCurrencyPlugin : public SourcePluginInterface
{
    Q_OBJECT
    Q_INTERFACES( SourcePluginInterface )
    Q_PLUGIN_METADATA(IID "com.mysoft.SourcePluginInterface" FILE "interface.json")
public:
    QString getName() const override
    {return "CBRF-Currency";}
    std::string getCurrencyCode() const override
    {return "RUB";}
    AbstractParser::Ptr createParser() const override
    {return AbstractParser::Ptr(new CbrfCurrencyParser);}
    QUrl getUrl() const override
    {return QUrl("http://www.cbr.ru/scripts/XML_daily.asp");}
};

#endif // SMARTLABPLUGIN_H
