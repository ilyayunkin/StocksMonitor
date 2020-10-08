#ifndef STOCKSMONITOR_H
#define STOCKSMONITOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "AbstractParser.h"
#include "Rules/AbstractStocksReceiver.h"
#include "Entities/StocksListHandler.h"

class AbstractStocksModel;

class StocksMonitor : public QObject
{
    Q_OBJECT
    AbstractStocksReceiver &model;
    const stocksListHandler handler;

    QByteArray time;
    QNetworkAccessManager m_WebCtrl;
    AbstractParser::Ptr parser;
    const QUrl url;
public:
    StocksMonitor(AbstractStocksReceiver &model,
                  const stocksListHandler handler,
                  AbstractParser::Ptr parser,
                  const QUrl url,
                  QObject *parent = nullptr);
    ~StocksMonitor();
public slots:
    void requestFileFromTheInternet();
    void fileDownloaded(QNetworkReply*r);

signals:    
    void downloaded(const QByteArray &time);
};

#endif // STOCKSMONITOR_H
