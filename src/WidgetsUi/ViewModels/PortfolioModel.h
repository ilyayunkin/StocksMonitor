#ifndef PORTFOLIO_MODEL_H
#define PORTFOLIO_MODEL_H

#include <vector>

#include <QAbstractTableModel>

#include "Rules/AbstractStocksView.h"
class PortfolioInterface;

class PortfolioModel : public QAbstractTableModel, public AbstractStocksView
{
    Q_OBJECT
public:
    enum
    {
        NAME,
        TICKER,
        QUANTITY,
        PRICE_BASE_CURRENCY,
        CURRENCY,
        SUM,
        DERIVATION,
        DERIVATION_WEEK,
        DERIVATION_MONTH,
        DERIVATION_YEAR,
        SELL_PRICE,

        COL_COUNT
    };
    explicit PortfolioModel(PortfolioInterface &portfolioInterface, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

signals:
    void updated();
private:
    mutable size_t size = 0;
    PortfolioInterface &portfolioInterface;
    void stocksUpdated() override;
    void stocksUpdated(size_t row) override;
};

#endif // PORTFOLIO_MODEL_H
