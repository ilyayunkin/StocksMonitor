#ifndef STRING_TYPES_H
#define STRING_TYPES_H

#include <string>
#include <QString>
#include <QByteArray>

typedef std::string Ticker;
typedef QString Url;
typedef QString StockName;
typedef QString PluginName;
typedef std::string CurrencyCode;
typedef std::string TimeString;

inline QString toQString(const std::string &s){return s.data();}
inline QString toQString(const QByteArray &s){return s.data();}
inline QString& toQString(QString &s){return s;}
inline const QString& toQString(const QString &s){return s;}
inline const char *toPlainText(const std::string &s){return s.c_str();}
inline const char *toPlainText(const QByteArray &s){return s;}

#endif // STRING_TYPES_H
