#ifndef StocksModel_H
#define StocksModel_H

#include <QAbstractTableModel>

#include <cstdlib>

#include "Application/StocksInterface.h"
#include "Rules/AbstractStocksView.h"
/**
 * @brief Класс Модели (архитектура Модель/Представление), реализующей
 * хранение вычисленных значений моментов.
 */
class StocksModel final: public QAbstractTableModel, public AbstractStocksView
{
    Q_OBJECT
public:
    enum
    {
        NAME,
        TICKER,
        PRICE,
        DERIVATION,
        DERIVATION_WEEK,
        DERIVATION_MONTH,
        DERIVATION_YEAR,

        COL_COUNT
    };
    explicit StocksModel(StocksInterface &stocksInterface, QObject *parent = 0);
    ~StocksModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void stocksUpdated() override;
    void stocksUpdated(size_t row) override;
signals:
    void time(std::string);
private:
    StocksInterface &stocks;
    mutable size_t size = 0;
};

#endif // StocksModel_H
