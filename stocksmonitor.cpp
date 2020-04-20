#include "stocksmonitor.h"

#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

#include <algorithm>
#include <assert.h>

#include "ExceptionClasses.h"
#include "SmartLabParser.h"

#define WRITE_DEBUG_FILES 0
#define DEBUG_PRINT 0


StocksMonitor::StocksMonitor(AbstractStocksModel &model, QObject *parent) : QObject(parent),
    model(model)
{
    connect(&m_WebCtrl, &QNetworkAccessManager::finished,
            this, &StocksMonitor::fileDownloaded);
    requestWordsFromTheInternet();
    {
        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout, this, &StocksMonitor::requestWordsFromTheInternet);
        t->start(5000);
    }
}


void StocksMonitor::requestWordsFromTheInternet()
{
#if DEBUG_PRINT
    qDebug() << __PRETTY_FUNCTION__;
#endif
    QNetworkRequest request(QUrl("https://smart-lab.ru/q/shares/order_by_last_to_prev_price/asc/"));
    m_WebCtrl.get(request);
}

void StocksMonitor::fileDownloaded(QNetworkReply *r)
{
    const QByteArray m_DownloadeAwholeDocumentdData = r->readAll();
    r->deleteLater();

#if DEBUG_PRINT
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "received" << m_DownloadeAwholeDocumentdData.size();
#endif
    try {
        QByteArray t;
        StocksList stocks;
        stocks.reserve(500);
        SmartLabParser::parse(m_DownloadeAwholeDocumentdData, stocks, t);

#if DEBUG_PRINT
        qDebug() << __PRETTY_FUNCTION__ << "time" << t;
#endif
        if(t == time)
        {
#if DEBUG_PRINT
            qDebug() << __PRETTY_FUNCTION__ << "skip";
#endif
        }else
        {
            time = t;
            model.setStocks(std::move(stocks));
            emit downloaded(time);
        }
    } catch (std::runtime_error &e) {
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << e.what();
    } catch (...) {
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "Unknown error";
    }
}
