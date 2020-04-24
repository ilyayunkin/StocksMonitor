#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>

class Logger
{
    QFile f;
    QTextStream s;
    Logger();
public:
    static Logger &instance();
    void log(const QString &text);
    QString getLog()const;
};

#endif // LOGGER_H
