#ifndef StocksModel_H
#define StocksModel_H

#include <QAbstractTableModel>
#include "abstractstocksmodel.h"
/**
 * @brief Класс Модели (архитектура Модель/Представление), реализующей
 * хранение вычисленных значений моментов.
 */
class StocksModel final: public QAbstractTableModel, public AbstractStocksModel
{
    Q_OBJECT

    const QString _pluginName;
    const QByteArray _currencyCode;
    StocksList stocks;
public:
    enum
    {
        NUM,
        NAME,
        TICKER,
        PRICE,
        DERIVATION,
        DERIVATION_WEEK,
        DERIVATION_MONTH,
        DERIVATION_YEAR,

        COL_COUNT
    };
    explicit StocksModel(const QString &plugin, const QByteArray &currencyCode, QObject *parent = 0);
    QString pluginName() const override;
    QByteArray currencyCode() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setStocks(StocksList &&stocks) override;
    size_t size() const override
    { return stocks.size(); }
    Stock getStock(const QByteArray &ticker) const override;
    Stock getStock(const size_t i) const override;
    float getStockPrice(const QByteArray &ticker) const override;
};

#endif // StocksModel_H
