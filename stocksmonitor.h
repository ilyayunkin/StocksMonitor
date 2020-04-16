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
    static QByteArray getDiv(const QByteArray &wholeDocument);
    static QByteArray getTable(const QByteArray &div);
    static QByteArrayList getRows(const QByteArray &table);
    static QByteArrayList getCols(const QByteArray &table);
    static QByteArray getA(const QByteArray &tableCol);
    static float getPercentage(const QByteArray &tableCol);
public:
    explicit StocksMonitor(AbstractStocksModel &model, QObject *parent = nullptr);
public slots:
    void requestWordsFromTheInternet();
    void fileDownloaded(QNetworkReply*r);

signals:    
    void downloaded(const QByteArray &time);
};

#endif // STOCKSMONITOR_H
