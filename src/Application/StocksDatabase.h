#ifndef StocksDatabase_H
#define StocksDatabase_H

#include <QAbstractTableModel>
#include "Rules/AbstractStocksModel.h"
#include "Rules/AbstractStocksView.h"

class StocksDatabase final: public AbstractStocksModel
{
    const QString _pluginName;
    const QByteArray _currencyCode;
    StocksList stocks;
    AbstractStocksView *view = nullptr;

public:
    explicit StocksDatabase(const QString &plugin,
                            const QByteArray &currencyCode);
    QString pluginName() const override;
    QByteArray currencyCode() const override;
    void setStocks(StocksList &&stocks) override;
    size_t size() const override
    { return stocks.size(); }
    Stock getStock(const QByteArray &ticker) const override;
    Stock getStock(const size_t i) const override;
    float getStockPrice(const QByteArray &ticker) const override;

    void setView(AbstractStocksView *view) override;
};

#endif // StocksDatabase_H
