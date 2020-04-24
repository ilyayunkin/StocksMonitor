#include "logger.h"

#include <QDebug>
#include <QDateTime>

Logger::Logger() :
    f{QString("log_%1.txt").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd"))},
    s{&f}
{
    f.open(QIODevice::Append);
}

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::log(const QString &text)
{
    s << text << "\r\n";
    qDebug() << text;
    s.flush();
}
