#include "CbrfMetalPlugin.h"
#include <QDateTime>

QUrl CbrfMetalPlugin::getUrl() const
{
    QDateTime current = QDateTime::currentDateTime();
    auto url = QString(R"URL(http://www.cbr.ru/scripts/xml_metall.asp?date_req1=%1/%2/%3&date_req2=%1/%2/%3)URL")
            .arg(current.date().day() - 1)
            .arg(current.date().month())
            .arg(current.date().year());
    return QUrl(url);
}
