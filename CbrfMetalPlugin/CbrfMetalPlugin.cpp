#include "CbrfMetalPlugin.h"
#include <QDate>

QUrl CbrfMetalPlugin::getUrl() const
{
    QDate current = QDate::currentDate();
    QDate yesterday = QDate::currentDate().addDays(-1);
    auto url = QString(R"URL(http://www.cbr.ru/scripts/xml_metall.asp?date_req1=%1/%2/%3&date_req2=%4/%5/%6)URL")
            .arg(yesterday.day())
            .arg(yesterday.month())
            .arg(yesterday.year())
            .arg(current.day())
            .arg(current.month())
            .arg(current.year());
    return QUrl(url);
}
