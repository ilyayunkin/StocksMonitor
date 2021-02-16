#ifndef SMARTLABPLUGIN_H
#define SMARTLABPLUGIN_H

#include "SourcePluginInterface.h"
#include "CbrfMetalParser.h"

class CbrfMetalPlugin : public SourcePluginInterface
{
    Q_OBJECT
    Q_INTERFACES( SourcePluginInterface )
    Q_PLUGIN_METADATA(IID "com.mysoft.SourcePluginInterface" FILE "interface.json")
public:
    PluginName getName() const override
    {return "CBRF-Metal";}
    CurrencyCode getCurrencyCode() const override
    {return "RUB";}
    AbstractParser::Ptr createParser() const override
    {return AbstractParser::Ptr(new CbrfMetalParser);}
    QUrl getUrl() const override;
};

#endif // SMARTLABPLUGIN_H
