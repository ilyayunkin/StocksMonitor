#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>

class Logger
{
public:
    static Logger &instance();
    void log(const QString &text);
    QString getLog()const;
private:
    QFile f;
    QTextStream s;
    Logger();
};

#endif // LOGGER_H
