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

    QNetworkAccessManager m_WebCtrl;
    QByteArray getDiv(const QByteArray &wholeDocument);
    QByteArray getTable(const QByteArray &div);
    QByteArrayList getRows(const QByteArray &table);
    QByteArrayList getCols(const QByteArray &table);
    QByteArray getA(const QByteArray &tableCol);
public:
    explicit StocksMonitor(AbstractStocksModel &model, QObject *parent = nullptr);
public slots:
    void requestWordsFromTheInternet();
    void fileDownloaded(QNetworkReply*r);

signals:    
    void downloaded();
};

#endif // STOCKSMONITOR_H
