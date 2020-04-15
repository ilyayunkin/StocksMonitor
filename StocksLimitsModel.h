#ifndef StocksLimitsModel_H
#define StocksLimitsModel_H

#include <QAbstractTableModel>
#include "abstractstocksmodel.h"
struct StockLimit
{
    QString name;
    QByteArray ticker;
    float price;
    float basePrice;

    StockLimit(const Stock &stock, float basePrice) :
        name(stock.name), ticker(stock.ticker), price(stock.price), basePrice(basePrice)
    {}
};
typedef QList<StockLimit> StockLimitsList;
/**
 * @brief Класс Модели (архитектура Модель/Представление), реализующей
 * хранение вычисленных значений моментов.
 */
class StocksLimitsModel : public QAbstractTableModel
{
    Q_OBJECT

    enum
    {
        NAME,
        TICKER,
        PRICE,
        BASE_PRICE,
        DISTANCE,

        COL_COUNT
    };


    StockLimitsList stockLimits;
    AbstractStocksModel *stocksModel = nullptr;

    void update();
public:
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
};

#endif // StocksLimitsModel_H
