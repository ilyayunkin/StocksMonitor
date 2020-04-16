#ifndef StocksLimitsModel_H
#define StocksLimitsModel_H

#include <QAbstractTableModel>

#include <QtSql>

#include <vector>

#include "abstractstocksmodel.h"

struct StockLimit
{
    QString name;
    QByteArray ticker;
    float price;
    float basePrice;
    StockLimit() = default;
    StockLimit(const Stock &stock, float basePrice) :
        name(stock.name), ticker(stock.ticker), price(stock.price), basePrice(basePrice)
    {}
    bool operator ==(const StockLimit &other)
    {
        return (ticker == other.ticker)
                && (basePrice == other.basePrice);
    }
};
typedef std::vector<StockLimit> StockLimitsList;

class StocksLimitsModel final: public QAbstractTableModel
{
    Q_OBJECT
    enum class Color
    {
        RED,
        YELLOW,
        GREEN,
        NO_COLOR
    };
    typedef std::vector<Color> ColorsList;

    QSqlDatabase db;

    StockLimitsList stockLimits;
    ColorsList colors;
    AbstractStocksModel *stocksModel = nullptr;

    static Color colorForDistance(float d)
    {
        if(d < 0.01)
            return Color::RED;
        else if(d < 0.05)
            return Color::YELLOW;
        else if(d < 0.1)
            return Color::GREEN;
        else
            return Color::NO_COLOR;
    }
    static Color colorForDistance(const StockLimit &limit)
    {
        float d = (limit.price - limit.basePrice) / limit.basePrice;
        return colorForDistance(d);
    }
    static QBrush brushForColor(const Color c)
    {
        switch (c) {
        case Color::RED: return QBrush(Qt::GlobalColor::red);
        case Color::YELLOW: return QBrush(Qt::GlobalColor::yellow);
        case Color::GREEN: return QBrush(Qt::GlobalColor::green);
        default:
        case Color::NO_COLOR: return QBrush();
        }
    }
    static float distance(const StockLimit &stock)
    {
        return (stock.price - stock.basePrice) / stock.basePrice;
    }

    void update();
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
    explicit StocksLimitsModel(QObject *parent = 0);
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

signals:
    void boundCrossed();
};

#endif // StocksLimitsModel_H
