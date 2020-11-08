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

class StatisticsTreeElement
{
public:
    StatisticsTreeElement(const StatisticsModelElementType type,
                          const QString &name);
    StatisticsTreeElement(const StatisticsConfigList &statisticsConfig);
    StatisticsTreeElement(const StatisticsCathegoryConfig &statistics);
    StatisticsTreeElement(const StatisticsGroupConfig &group);
    StatisticsTreeElement(const QString &stock, const char *const ticker);
    ~StatisticsTreeElement();
    const QByteArray &getTicker() const {return ticker;}
    const QString &getName() const {return name;}
    StatisticsModelElementType getType() const {return type;}
    const StatisticsTreeElement *getParent(){return parent;}
    const StatisticsTreeElement *getParent()const {return parent;}
    size_t getChildCount()const {return children.size();}
    StatisticsTreeElement *getChild(const size_t row){return children[row];}
    const StatisticsTreeElement *getChild(const size_t row) const{return children[row];}
    void reserve(const int s);
    void add(StatisticsTreeElement *element);
    void remove(StatisticsTreeElement *element);
    int getRow() const;
private:
    const StatisticsModelElementType type;
    const QString name;
    const QByteArray ticker;
    const StatisticsTreeElement *parent = nullptr;
    std::vector<StatisticsTreeElement *> children;
};

class StatisticsModelPrivate;
class StatisticsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    StatisticsModel(const StatisticsConfigList &config,
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
    StatisticsTreeElement *getTreeElement(const QModelIndex &index);

    void addCategory(const QString &name);
    void addGroup(StatisticsTreeElement *parentItem, const QString &name);
    void addItem(StatisticsTreeElement *parentItem, const QString &name,
                 const char *const &ticker);
    void remove(StatisticsTreeElement *item);
private:
    std::shared_ptr<StatisticsModelPrivate> pimpl;
    QModelIndex index(const StatisticsTreeElement * const item);
};

#endif // STATISTICSMODEL_H
