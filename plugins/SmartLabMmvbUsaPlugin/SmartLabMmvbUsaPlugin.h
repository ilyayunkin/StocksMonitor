#ifndef SMARTLABPLUGIN_H
#define SMARTLABPLUGIN_H

#include "SourcePluginInterface.h"
#include "SmartLabMmvbUsaParser.h"

class SmartLabMmvbUsaPlugin : public SourcePluginInterface
{
    Q_OBJECT
    Q_INTERFACES( SourcePluginInterface )
    Q_PLUGIN_METADATA(IID "com.mysoft.SourcePluginInterface" FILE "interface.json")
public:
    PluginName getName() const override
    {return "Smart-Lab-MMVB-Usa";}
    CurrencyCode getCurrencyCode() const override
    {return "RUB";}
    AbstractParser::Ptr createParser() const override
    {return AbstractParser::Ptr(new SmartLabMmvbUsaParser);}
    QUrl getUrl() const override
    {return QUrl("https://smart-lab.ru/q/usa-stocks-moex/order_by_last_to_prev_price/asc/");}
};

#endif // SMARTLABPLUGIN_H
