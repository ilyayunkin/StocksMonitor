#include "logger.h"

#include <assert.h>

#include <QDebug>
#include <QDateTime>
#include <QTextCodec>

#include "ExceptionClasses.h"

Logger::Logger() :
    f{QString("log_%1.txt").arg(QDateTime::currentDateTime().toString("yyyy.MM.dd"))},
    s{&f}
{
    f.open(QIODevice::Append);
    s.setCodec("UTF-8");
    assert(s.codec()->name() == "UTF-8");
}

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::log(const QString &text)
{
    s << QDateTime::currentDateTime().toString("yyyy.MM.dd")
      << ' '
      << text << "\r\n";
    qDebug() << text;
    s.flush();
}

QString Logger::getLog() const
{
    QFile readFile (f.fileName());
    if(!readFile.open(QIODevice::ReadOnly))
    {
        throw FileUnavailibleException();
    }

    QString str = readFile.readAll();
    readFile.close();

    return str;
}
