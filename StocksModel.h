#ifndef StocksModel_H
#define StocksModel_H

#include <QAbstractTableModel>
#include "abstractstocksmodel.h"
/**
 * @brief Класс Модели (архитектура Модель/Представление), реализующей
 * хранение вычисленных значений моментов.
 */
class StocksModel : public QAbstractTableModel, public AbstractStocksModel
{
    Q_OBJECT

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

    StocksList stocks;
public:
    explicit StocksModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setStocks(StocksList &&stocks) override;
};

#endif // StocksModel_H
