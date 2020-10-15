#include "StatisticsModel.h"

#include <QDebug>

#include <algorithm>
#include <assert.h>

namespace  {
class TreeElement
{
    const StatisticsModelElementType type;
    const QString name;
    const QByteArray ticker;
    const TreeElement *parent = nullptr;
    std::vector<TreeElement *> children;
public:
    const QByteArray &getTicker() const {return ticker;}
    const QString &getName() const {return name;}
    StatisticsModelElementType getType() const {return type;}
    const TreeElement *getParent(){return parent;}
    const TreeElement * getParent()const {return parent;}
    size_t getChildCount()const {return children.size();}
    TreeElement *getChild(const size_t row){return children[row];}
    const TreeElement *getChild(const size_t row) const{return children[row];}

    TreeElement(const StatisticsModelElementType type,
                const QString &name)
        : type(type)
        , name(name)
    {
        assert(type != StatisticsModelElementType::STOCK);
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << name;
    }
    TreeElement(const StatisticsConfigList &statisticsConfig)
        : type(StatisticsModelElementType::ROOT)
    {
        reserve(statisticsConfig.size());
        for(const auto &stat : statisticsConfig){
            add(new TreeElement(stat));
        }
    }
    TreeElement(const StatisticsCathegoryConfig &statistics)
        : type(StatisticsModelElementType::CATEGORY)
        , name(statistics.name)
    {
        reserve(statistics.list.size());
        for(const auto &group : statistics.list){
            add(new TreeElement(group));
        }
    }
    TreeElement(const StatisticsGroupConfig &group)
        : type(StatisticsModelElementType::GROUP)
        , name(group.name)
    {
        reserve(group.list.size());
        for(const auto &stock : group.list){
            add(new TreeElement(stock.name, stock.ticker.data()));
        }
    }
    TreeElement(const QString &stock, const char *const ticker)
        : type(StatisticsModelElementType::STOCK)
        , name(stock)
        , ticker(ticker)
    {
        assert(ticker);
        assert(strlen(ticker) != 0);
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << int(type) << name << ticker;
    }
    ~TreeElement()
    {
        qDebug() << __PRETTY_FUNCTION__ << __LINE__ << name;
        std::destroy(children.begin(), children.end());
    }
    void add(TreeElement *element)
    {
        element->parent = this;
        children.push_back(element);
    }
    void remove(TreeElement *element)
    {
        children.erase(std::remove(children.begin(), children.end(), element));
        delete element;
    }
    int getRow() const
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
    void reserve(const int s)
    {
        children.reserve(s);
    }
};
}

struct StatisticsModelPrivate
{
    StatisticsController &interface;
    TreeElement root;
    StatisticsModelPrivate(StatisticsController &interface)
        : interface(interface)
        , root(interface.getStatisticsConfig())
    {
    }
};

StatisticsModel::StatisticsModel(StatisticsController &interface, QObject *parent)
    : QAbstractItemModel(parent)
    , interface(interface)
    , pimpl(new StatisticsModelPrivate(interface))
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
        auto item = static_cast<TreeElement *>(index.internalPointer());
        return item->getType();
    }
    return StatisticsModelElementType::NONE;
}

void StatisticsModel::addCategory(const QModelIndex &parent, const QString &name)
{
    if(parent.isValid())
    {
        auto parentItem = static_cast<TreeElement *>(parent.internalPointer());
        assert(parentItem);
        assert(parentItem->getType() == StatisticsModelElementType::ROOT);

        beginInsertRows(parent, parentItem->getChildCount(), parentItem->getChildCount());
        parentItem->add(new TreeElement(StatisticsModelElementType::CATEGORY, name));
        interface.addCategory(name);
        endInsertRows();
    }
}

void StatisticsModel::addGroup(const QModelIndex &parent, const QString &name)
{
    if(parent.isValid())
    {
        auto parentItem = static_cast<TreeElement *>(parent.internalPointer());
        assert(parentItem);
        assert(parentItem->getType() == StatisticsModelElementType::CATEGORY);

        beginInsertRows(parent, parentItem->getChildCount(), parentItem->getChildCount());
        parentItem->add(new TreeElement(StatisticsModelElementType::GROUP, name));
        interface.addGroup(parentItem->getName(), name);
        endInsertRows();
    }
}

void StatisticsModel::addItem(const QModelIndex &parent, const QString &name, const char * const &ticker)
{
    assert(ticker);
    assert(strlen(ticker) != 0);
    if(parent.isValid())
    {
        auto parentItem = static_cast<TreeElement *>(parent.internalPointer());
        assert(parentItem);
        assert(parentItem->getType() == StatisticsModelElementType::GROUP);

        beginInsertRows(parent, parentItem->getChildCount(), parentItem->getChildCount());
        if(interface.addItem(parentItem->getParent()->getName()
                          , parentItem->getName()
                          , StockId(ticker, name)))
        {
            parentItem->add(new TreeElement(name, ticker));
        }
        endInsertRows();
    }
}

void StatisticsModel::remove(const QModelIndex &index)
{
    auto item = static_cast<TreeElement *>(index.internalPointer());
    assert(item);

    switch (item->getType())
    {
    case StatisticsModelElementType::CATEGORY:
    {
        bool ok = interface.removeCategory(item->getName());
        if(ok)
        {
            beginRemoveRows(index.parent(), item->getRow(), item->getRow());
            const_cast<TreeElement *>(item->getParent())->remove(item);
            endRemoveRows();
        }
    }
        break;
    case StatisticsModelElementType::GROUP:
    {
        bool ok = interface.removeGroup(item->getParent()->getName(), item->getName());
        if(ok)
        {
            beginRemoveRows(index.parent(), item->getRow(), item->getRow());
            const_cast<TreeElement *>(item->getParent())->remove(item);
            endRemoveRows();
        }
    }
        break;
    case StatisticsModelElementType::STOCK:
    {
        auto const category = item->getParent()->getParent()->getName();
        auto const group = item->getParent()->getName();
        auto const ticker = item->getTicker();
        assert(!ticker.isEmpty());
        bool ok = interface.removeItem(category, group, ticker);
        if(ok)
        {
            beginRemoveRows(index.parent(), item->getRow(), item->getRow());
            const_cast<TreeElement *>(item->getParent())->remove(item);
            endRemoveRows();
        }
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
        auto parentItem = static_cast<TreeElement *>(parent.internalPointer());
        assert(parentItem);

        return parentItem->getChildCount();
    }
}

QModelIndex StatisticsModel::parent(const QModelIndex &index) const
{
    auto item = static_cast<TreeElement *>(index.internalPointer());
    if(item == nullptr)
    {
        return QModelIndex();
    }
    if(item->getParent() == nullptr)
    {
        return QModelIndex();
    }

    auto parentItem = item->getParent();

    return createIndex(parentItem->getRow(), 0, const_cast<TreeElement *>(parentItem));
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
        auto parentItem = static_cast<TreeElement *>(parent.internalPointer());
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
        auto item = static_cast<TreeElement *>(index.internalPointer());

        return item->getName();
    }
    return QVariant();
}
