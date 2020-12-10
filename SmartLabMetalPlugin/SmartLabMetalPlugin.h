#ifndef SMARTLABPLUGIN_H
#define SMARTLABPLUGIN_H

#include "SourcePluginInterface.h"
#include "SmartLabMetalParser.h"

class SmartLabMetalPlugin : public SourcePluginInterface
{
    Q_OBJECT
    Q_INTERFACES( SourcePluginInterface )
    Q_PLUGIN_METADATA(IID "com.mysoft.SourcePluginInterface" FILE "interface.json")
public:
    PluginName getName() const override
    {return "Smart-Lab-Metal";}
    CurrencyCode getCurrencyCode() const override
    {return "USD";}
    AbstractParser::Ptr createParser() const override
    {return AbstractParser::Ptr(new SmartLabMetalParser);}
    QUrl getUrl() const override
    {return QUrl("https://smart-lab.ru/q/world-quotes/");}
};

#endif // SMARTLABPLUGIN_H
