#ifndef STOCKSMONITOR_H
#define STOCKSMONITOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "abstractstocksmodel.h"

class StocksMonitor : public QObject
{
    Q_OBJECT
    AbstractStocksModel &model;

    QByteArray time;
    QNetworkAccessManager m_WebCtrl;
public:
    explicit StocksMonitor(AbstractStocksModel &model, QObject *parent = nullptr);
public slots:
    void requestWordsFromTheInternet();
    void fileDownloaded(QNetworkReply*r);

signals:    
    void downloaded(const QByteArray &time);
};

#endif // STOCKSMONITOR_H
