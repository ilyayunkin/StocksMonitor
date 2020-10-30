#ifndef STOCKSMONITOR_H
#define STOCKSMONITOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "AbstractParser.h"
#include "Rules/AbstractStocksReceiver.h"
#include "Entities/StocksListHandler.h"

class StocksMonitor : public QObject
{
    Q_OBJECT
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
    void downloaded(const std::string &time);
private:
    AbstractStocksReceiver &model;
    const stocksListHandler handler;

    std::string time;
    QNetworkAccessManager m_WebCtrl;
    AbstractParser::Ptr parser;
    const QUrl url;
};

#endif // STOCKSMONITOR_H
