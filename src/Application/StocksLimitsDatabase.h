#ifndef StocksLimitsDatabase_H
#define StocksLimitsDatabase_H

#include <QtSql>

#include <vector>

#include "StocksList.h"
#include "Rules/StockLimit.h"
#include "Rules/AbstractStocksView.h"
#include "Color.h"

class AbstractStocksModel;
class QTimer;

class StocksLimitsDatabase : public QObject
{
    Q_OBJECT
    typedef std::vector<Color> ColorsList;

    QSqlDatabase db;
    QTimer *t;

    StockLimitsList stockLimits;
    ColorsList colors;
    AbstractStocksModel *stocksModel = nullptr;
    QString name = "noname";
    AbstractStocksView *view = nullptr;

    static float distance(const StockLimit &stock)
    {
        return (stock.price - stock.basePrice) / stock.basePrice;
    }

    QSqlQuery executeQuery(const QString &query);
public:
    explicit StocksLimitsDatabase(QString name, QObject *parent = 0);
    ~StocksLimitsDatabase();
    size_t size() const;

    void setStocksModel(AbstractStocksModel *stocksModel);
    void addStock(const StockLimit &stockLimit);
    StockLimitsList getList() const {return stockLimits;}
    QBrush getBrush(int row) const;
    StockLimit getLimit(int row) const;
    QString getToolTip(int row) const;
    bool setReferencePrice(int row, float referencePrice);

    void setView(AbstractStocksView *view);
public slots:
    void update();
signals:
    void boundCrossed();
    void crossedLimit(const StockLimit &stockLimit);
};

#endif // StocksLimitsDatabase_H
