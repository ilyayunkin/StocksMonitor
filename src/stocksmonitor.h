#ifndef STOCKSMONITOR_H
#define STOCKSMONITOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "AbstractParser.h"

class AbstractStocksModel;

class StocksMonitor : public QObject
{
    Q_OBJECT
    AbstractStocksModel &model;

    QByteArray time;
    QNetworkAccessManager m_WebCtrl;
    AbstractParser::Ptr parser;
    const QUrl url;
public:
    explicit StocksMonitor(AbstractStocksModel &model,
                           AbstractParser::Ptr parser,
                           const QUrl url,
                           QObject *parent = nullptr);
public slots:
    void requestFileFromTheInternet();
    void fileDownloaded(QNetworkReply*r);

signals:    
    void downloaded(const QByteArray &time);
};

#endif // STOCKSMONITOR_H
