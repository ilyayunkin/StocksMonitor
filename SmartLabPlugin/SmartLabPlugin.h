#ifndef SMARTLABPLUGIN_H
#define SMARTLABPLUGIN_H

#include "SourcePluginInterface.h"
#include "SmartLabParser.h"

class SmartLabPlugin : public SourcePluginInterface
{
    Q_OBJECT
    Q_INTERFACES( SourcePluginInterface )
    Q_PLUGIN_METADATA(IID "com.mysoft.SourcePluginInterface" FILE "interface.json")
public:
    QString getName() const override
    {return "Smart-Lab";}
    AbstractParser::Ptr createParser() const override
    {return AbstractParser::Ptr(new SmartLabParser);}
    QUrl getUrl() const override
    {return QUrl("https://smart-lab.ru/q/shares/order_by_last_to_prev_price/asc/");}
};

#endif // SMARTLABPLUGIN_H
