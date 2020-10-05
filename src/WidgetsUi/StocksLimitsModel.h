#ifndef StocksLimitsModel_H
#define StocksLimitsModel_H

#include <QAbstractTableModel>

#include <QtSql>

#include <vector>

#include "StocksList.h"
#include "Rules/StockLimit.h"
#include "Color.h"

class AbstractStocksModel;

class StocksLimitsModel final: public QAbstractTableModel
{
    Q_OBJECT
    typedef std::vector<Color> ColorsList;

    QSqlDatabase db;

    StockLimitsList stockLimits;
    ColorsList colors;
    AbstractStocksModel *stocksModel = nullptr;
    QString name = "noname";

    static float distance(const StockLimit &stock)
    {
        return (stock.price - stock.basePrice) / stock.basePrice;
    }

    QSqlQuery executeQuery(const QString &query);
public:
    enum
    {
        NAME,
        TICKER,
        PRICE,
        BASE_PRICE,
        DISTANCE,

        COL_COUNT
    };
    explicit StocksLimitsModel(QString name,
                               bool autoupdate = true,
                               QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void setStocksModel(AbstractStocksModel *stocksModel);
    void addStock(const StockLimit &stockLimit);
    StockLimitsList getList() const {return stockLimits;}

public slots:
    void update();
signals:
    void boundCrossed();
    void crossedLimit(const StockLimit &stockLimit);
};

#endif // StocksLimitsModel_H
