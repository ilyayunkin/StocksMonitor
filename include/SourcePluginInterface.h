#ifndef SOURCEPLUGININTERFACE_H
#define SOURCEPLUGININTERFACE_H

#include <QString>
#include <QVariant>
#include <QUrl>
#include <QtPlugin>

#include "AbstractParser.h"

class SourcePluginInterface : public QObject
{
    Q_OBJECT
public:
    virtual ~SourcePluginInterface() { }

    virtual QString getName() const = 0;
    virtual AbstractParser::Ptr createParser() const = 0;
    virtual QUrl getUrl() const = 0;
};

Q_DECLARE_INTERFACE( SourcePluginInterface, "com.mysoft.SourcePluginInterface" )

#endif // SOURCEPLUGININTERFACE_H
