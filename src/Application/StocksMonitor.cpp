#include "StocksMonitor.h"

#include <QFile>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

#include <algorithm>
#include <assert.h>

#include "ExceptionClasses.h"
#include "logger.h"

#define WRITE_DEBUG_FILES 0
#define DEBUG_PRINT 0

StocksMonitor::StocksMonitor(AbstractStocksReceiver &model,
                             const stocksListHandler handler,
                             AbstractParser::Ptr parser,
                             const QUrl url,
                             QObject *parent) :
    QObject(parent),
    model(model),
    handler(handler),
    parser(std::move(parser)),
    url(url)
{
    assert(this->parser.get() != nullptr);

    {
        bool ssl = QSslSocket::supportsSsl();
        qDebug() << "Supports SSL: " << ssl
                 << QSslSocket::sslLibraryBuildVersionString()
                 << QSslSocket::sslLibraryVersionString();

        if(!ssl)
        {
            throw DistributionCorruptedException();
        }
    }

    connect(&m_WebCtrl, &QNetworkAccessManager::finished,
            this, &StocksMonitor::fileDownloaded);
    requestFileFromTheInternet();
    {
        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout, this, &StocksMonitor::requestFileFromTheInternet);
        t->start(5000);
    }
}

StocksMonitor::~StocksMonitor()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void StocksMonitor::requestFileFromTheInternet()
{
#if DEBUG_PRINT
    qDebug() << __PRETTY_FUNCTION__;
#endif
    QNetworkRequest request(url);
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
        std::string t;
        StocksList stocks;
        static int reserveSize = 500;
        stocks.reserve(reserveSize);
        parser->parse(m_DownloadeAwholeDocumentdData, stocks, t);
        reserveSize = std::max<int>(reserveSize, stocks.size());

#if DEBUG_PRINT
        qDebug() << __PRETTY_FUNCTION__ << "time" << t;
#endif
        if(t.empty())
        {
            throw EmptyTimestampException();
        }

        if(t == time)
        {
#if DEBUG_PRINT
            qDebug() << __PRETTY_FUNCTION__ << "skip";
#endif
        }else
        {
            time = t;
            model.setStocks(handler, std::move(stocks), time.data());
            emit downloaded(time);
        }
    } catch (std::runtime_error &e) {
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << e.what();
        Logger::instance().log(QDateTime::currentDateTime().toString()
                               + __PRETTY_FUNCTION__
                               + e.what());
    } catch (...) {
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << "Unknown error";
        Logger::instance().log(QDateTime::currentDateTime().toString()
                               + __PRETTY_FUNCTION__
                               + "Unknown error");
    }
}
