#ifndef POCKETMODEL_H
#define POCKETMODEL_H

#include <AbstractPocket.h>

#include <vector>

#include <QAbstractTableModel>

#include <QtSql>

#include "ModelsReference.h"

class PocketModel : public QAbstractTableModel, public AbstractPocket
{
    Q_OBJECT

    struct PocketEntry
    {
        QString plugin;
        QString name;
        QByteArray ticker;
        int quantity;
        double priceBaseCurrency;
        double sum;
        QByteArray currency;
        StocksModel *model;
    };
    typedef std::vector<PocketEntry> PocketEntryList;
    PocketEntryList entries;
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

        COL_COUNT
    };
    explicit PocketModel(ModelsReferenceList &models, QObject *parent = 0);
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
};

#endif // POCKETMODEL_H
