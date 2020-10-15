#ifndef STATISTICSMODEL_H
#define STATISTICSMODEL_H

#include <QAbstractItemModel>
#include <memory>
#include "Application/Controllers/StatisticsController.h"

enum class StatisticsModelElementType
{
    ROOT,
    CATEGORY,
    GROUP,
    STOCK,

    NONE
};

class StatisticsModelPrivate;
class StatisticsModel : public QAbstractItemModel
{
    Q_OBJECT

    StatisticsController &interface;
    std::shared_ptr<StatisticsModelPrivate> pimpl;
public:
    StatisticsModel(StatisticsController &interface,
                    QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    StatisticsModelElementType getType(const QModelIndex &index);

    void addCategory(const QModelIndex &parent, const QString &name);
    void addGroup(const QModelIndex &parent, const QString &name);
    void addItem(const QModelIndex &parent, const QString &name,
                 const char *const &ticker);
    void remove(const QModelIndex &index);
};

#endif // STATISTICSMODEL_H
