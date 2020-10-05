#ifndef POCKETMODEL_H
#define POCKETMODEL_H

#include "Rules/AbstractPocket.h"

#include <vector>

#include <QAbstractTableModel>

#include <QtSql>

#include "Rules/ModelsReference.h"

struct PortfolioEntry
{
    QString plugin;
    QString name;
    QByteArray ticker;
    int quantity;
    float price;
    float sellPrice;
    float sum;
    QByteArray currency;
    AbstractStocksModel *model;
};
typedef std::vector<PortfolioEntry> PortfolioEntryList;

class PortfolioModel : public QAbstractTableModel, public AbstractPocket
{
    Q_OBJECT

    PortfolioEntryList entries;
    QSqlDatabase db;
    ModelsReferenceList &models;
    CurrencyCountersList currencyCounters;

    QSqlQuery executeQuery(const QString &query);
    void update();
public:
    enum
    {
        NAME,
        TICKER,
        QUANTITY,
        PRICE_BASE_CURRENCY,
        CURRENCY,
        SUM,
        SELL_PRICE,

        COL_COUNT
    };
    explicit PortfolioModel(ModelsReferenceList &models, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    Qt::ItemFlags flags(const QModelIndex & index) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void addStock(QString plugin, QByteArray ticker, int quantity) override;
    CurrencyCountersList sum() const override;
signals:
    void boundCrossed();
    void crossedLimit(const PortfolioEntry &stockLimit);
};

#endif // POCKETMODEL_H
