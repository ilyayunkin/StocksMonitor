#ifndef STOCKSMONITOR_H
#define STOCKSMONITOR_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>

struct Stock
{
    int rowNum;
    QString name;
    QString ticker;
    double price;
};
typedef QList<Stock> StocksList;

class StocksMonitor : public QObject
{
    Q_OBJECT
    QNetworkAccessManager m_WebCtrl;
    QByteArray getDiv(const QByteArray &wholeDocument);
    QByteArray getTable(const QByteArray &div);
    QByteArrayList getRows(const QByteArray &table);
    QByteArrayList getCols(const QByteArray &table);
    QByteArray getA(const QByteArray &tableCol);
public:
    explicit StocksMonitor(QObject *parent = nullptr);
public slots:
    void requestWordsFromTheInternet();
    void fileDownloaded(QNetworkReply*r);

signals:    
    void downloaded();
};

#endif // STOCKSMONITOR_H
