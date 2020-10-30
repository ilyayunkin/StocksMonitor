#include "StatisticsModel.h"

#include <QDebug>

#include <algorithm>
#include <assert.h>

StatisticsTreeElement::StatisticsTreeElement(const StatisticsModelElementType type,
                                             const QString &name)
    : type(type)
    , name(name)
{
    assert(type != StatisticsModelElementType::STOCK);
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << name;
}
StatisticsTreeElement::StatisticsTreeElement(const StatisticsConfigList &statisticsConfig)
    : type(StatisticsModelElementType::ROOT)
{
    reserve(statisticsConfig.size());
    for(const auto &stat : statisticsConfig){
        add(new StatisticsTreeElement(stat));
    }
}
StatisticsTreeElement::StatisticsTreeElement(const StatisticsCathegoryConfig &statistics)
    : type(StatisticsModelElementType::CATEGORY)
    , name(statistics.name)
{
    reserve(statistics.list.size());
    for(const auto &group : statistics.list){
        add(new StatisticsTreeElement(group));
    }
}
StatisticsTreeElement::StatisticsTreeElement(const StatisticsGroupConfig &group)
    : type(StatisticsModelElementType::GROUP)
    , name(group.name)
{
    reserve(group.list.size());
    for(const auto &stock : group.list){
        add(new StatisticsTreeElement(stock.name, stock.ticker.data()));
    }
}
StatisticsTreeElement::StatisticsTreeElement(const QString &stock, const char *const ticker)
    : type(StatisticsModelElementType::STOCK)
    , name(stock)
    , ticker(ticker)
{
    assert(ticker);
    assert(strlen(ticker) != 0);
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << int(type) << name << ticker;
}
StatisticsTreeElement::~StatisticsTreeElement()
{
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << name;
    std::destroy(children.begin(), children.end());
}
void StatisticsTreeElement::add(StatisticsTreeElement *element)
{
    element->parent = this;
    children.push_back(element);
}
void StatisticsTreeElement::remove(StatisticsTreeElement *element)
{
    children.erase(std::remove(children.begin(), children.end(), element));
    delete element;
}
int StatisticsTreeElement::getRow() const
{
    if(parent == nullptr)
        return 0;

    int row = 0;
    for(auto item : parent->children)
    {
        if(item == this)
            return row;

        ++row;
    }

    return 0;
}
void StatisticsTreeElement::reserve(const int s)
{
    children.reserve(s);
}

struct StatisticsModelPrivate
{
    StatisticsTreeElement root;
    StatisticsModelPrivate(const StatisticsConfigList &config)
        : root(config)
    {
    }
};

StatisticsModel::StatisticsModel(const StatisticsConfigList &config, QObject *parent)
    : QAbstractItemModel(parent)
    , pimpl(new StatisticsModelPrivate(config))
{    
}


int StatisticsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

StatisticsModelElementType StatisticsModel::getType(const QModelIndex &index)
{
    if(index.isValid())
    {
        auto item = static_cast<StatisticsTreeElement *>(index.internalPointer());
        return item->getType();
    }
    return StatisticsModelElementType::NONE;
}

StatisticsTreeElement *StatisticsModel::getTreeElement(const QModelIndex &index)
{
    assert(index.isValid());

    auto item = static_cast<StatisticsTreeElement *>(index.internalPointer());
    return item;
}

void StatisticsModel::addCategory(const QString &name)
{
    beginInsertRows(parent(index(&pimpl->root)), pimpl->root.getChildCount(), pimpl->root.getChildCount());
    pimpl->root.add(new StatisticsTreeElement(StatisticsModelElementType::CATEGORY, name));
    endInsertRows();
}

void StatisticsModel::addGroup(StatisticsTreeElement *parentItem, const QString &name)
{
    assert(parentItem);
    assert(parentItem->getType() == StatisticsModelElementType::CATEGORY);

    beginInsertRows(index(parentItem), parentItem->getChildCount(), parentItem->getChildCount());
    parentItem->add(new StatisticsTreeElement(StatisticsModelElementType::GROUP, name));
    endInsertRows();
}

void StatisticsModel::addItem(StatisticsTreeElement *parentItem, const QString &name, const char * const &ticker)
{
    assert(ticker);
    assert(strlen(ticker) != 0);

    assert(parentItem);
    assert(parentItem->getType() == StatisticsModelElementType::GROUP);

    beginInsertRows(index(parentItem), parentItem->getChildCount(), parentItem->getChildCount());
    parentItem->add(new StatisticsTreeElement(name, ticker));
    endInsertRows();
}

void StatisticsModel::remove(StatisticsTreeElement *item)
{
    assert(item);

    auto const parentItem = item->getParent();
    switch (item->getType())
    {
    case StatisticsModelElementType::CATEGORY:
    {
        beginRemoveRows(index(parentItem), item->getRow(), item->getRow());
        const_cast<StatisticsTreeElement *>(item->getParent())->remove(item);
        endRemoveRows();
    }
        break;
    case StatisticsModelElementType::GROUP:
    {
        beginRemoveRows(index(parentItem), item->getRow(), item->getRow());
        const_cast<StatisticsTreeElement *>(item->getParent())->remove(item);
        endRemoveRows();
    }
        break;
    case StatisticsModelElementType::STOCK:
    {
        auto const category = item->getParent()->getParent()->getName();
        auto const group = item->getParent()->getName();
        auto const ticker = item->getTicker();
        assert(!ticker.isEmpty());

        beginRemoveRows(index(parentItem), item->getRow(), item->getRow());
        const_cast<StatisticsTreeElement *>(item->getParent())->remove(item);
        endRemoveRows();
    }
        break;
    case StatisticsModelElementType::ROOT:
    case StatisticsModelElementType::NONE:
    default:
        assert(0);
        break;
    }
}

int StatisticsModel::rowCount(const QModelIndex &parent) const
{
    // Root elements don't have parents
    const bool isRootElement = !parent.isValid();
    if(isRootElement)
    {
        return 1; // we suppose to have only 1 root
    }else
    {
        auto parentItem = static_cast<StatisticsTreeElement *>(parent.internalPointer());
        assert(parentItem);

        return parentItem->getChildCount();
    }
}

QModelIndex StatisticsModel::parent(const QModelIndex &index) const
{
    auto item = static_cast<StatisticsTreeElement *>(index.internalPointer());
    if(item == nullptr)
    {
        return QModelIndex();
    }
    if(item->getParent() == nullptr)
    {
        return QModelIndex();
    }

    auto parentItem = item->getParent();

    return createIndex(parentItem->getRow(), 0, const_cast<StatisticsTreeElement *>(parentItem));
}

QModelIndex StatisticsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    if (!parent.isValid())
    {
        return createIndex(row, column, &(pimpl->root));
    }
    else
    {
        auto parentItem = static_cast<StatisticsTreeElement *>(parent.internalPointer());
        assert(parentItem);
        auto childrenItem = parentItem->getChild(row);
        return createIndex(row, column, childrenItem);
    }
}

QVariant StatisticsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);
    return QVariant();
}

Qt::ItemFlags StatisticsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    if(parent(index).isValid())
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable /*| Qt::ItemIsEditable*/ | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    }else
    {
        return  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDropEnabled;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant StatisticsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        const bool isRootElement = !index.parent().isValid();
        if(isRootElement)
        {
            return tr("Stats");
        }
        auto item = static_cast<StatisticsTreeElement *>(index.internalPointer());

        return item->getName();
    }
    return QVariant();
}

QModelIndex StatisticsModel::index(const StatisticsTreeElement *const item)
{
    if(item == nullptr)
    {
        return QModelIndex();
    }else
    {
        return index(item->getRow(), 0, index(item->getParent()));
    }
}
