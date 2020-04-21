#include "logger.h"

Logger::Logger() :
    f{QString("log.txt")},
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
    s.flush();
}
