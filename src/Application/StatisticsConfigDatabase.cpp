#include "StatisticsConfigDatabase.h"
#include "ExceptionClasses.h"

namespace  {
constexpr auto categoryTableName = "category";
constexpr auto groupTableName = "groups";
constexpr auto itemsTableName = "items";
constexpr auto dbName = "StatisticsConfig";
}

QStringList StatisticsConfigDatabase::getAllCategories()const
{
    QStringList ret;
    QSqlQuery q = executeQueryException(
                QString("SELECT * FROM %1"
                        ";")
                .arg(categoryTableName));
    QSqlRecord rec = q.record();
    while(q.next())
    {
        ret.push_back(q.value(rec.indexOf("name")).toString());
    }
    return ret;
}

QStringList StatisticsConfigDatabase::getAllGroups(const QString &category)const
{
    QStringList ret;
    QSqlQuery q = executeQueryException(
                QString("SELECT * FROM %1 "
                        " WHERE category='%2'"
                        ";")
                .arg(groupTableName)
                .arg(category));
    QSqlRecord rec = q.record();
    while(q.next())
    {
        ret.push_back(q.value(rec.indexOf("name")).toString());
    }
    qDebug() << ret;
    return ret;
}

StockIdList StatisticsConfigDatabase::getAllItems(const QString &group)const
{
    StockIdList ret;
    QSqlQuery q = executeQueryException(
                QString("SELECT * FROM %1 "
                        " WHERE groupName='%2'"
                        ";")
                .arg(itemsTableName)
                .arg(group));
    QSqlRecord rec = q.record();
    while(q.next())
    {
        ret.push_back(StockId(q.value(rec.indexOf("ticker")).toByteArray().data(),
                              q.value(rec.indexOf("name")).toString()));
    }
    return ret;
}

void StatisticsConfigDatabase::addCategory(const QString &category)
{
    QSqlQuery q = executeQueryException(
                QString("INSERT INTO %1"
                        "(name) "
                        "VALUES('%2')"
                        ";")
                .arg(categoryTableName)
                .arg(category));
}

void StatisticsConfigDatabase::addGroup(const QString &category,
                                        const QString &group)
{
    QSqlQuery q = executeQueryException(
                QString("INSERT INTO %1"
                        "(category, name) "
                        "VALUES('%2', '%3')"
                        ";")
                .arg(groupTableName)
                .arg(category)
                .arg(group));
}

void StatisticsConfigDatabase::addItem(const QString &group, const StockId &item)
{
    QSqlQuery q = executeQueryException(
                QString("INSERT INTO %1"
                        "(groupName, ticker, name) "
                        "VALUES('%2', '%3', '%4')"
                        ";")
                .arg(itemsTableName)
                .arg(group)
                .arg(item.ticker.data())
                .arg(item.name));
}

void StatisticsConfigDatabase::removeCategory(const QString &category)
{
    {
        QSqlQuery q = executeQueryException("BEGIN TRANSACTION;");
    }
    try
    {
        {
            QSqlQuery q = executeQueryException(
                        QString("DELETE FROM %1"
                                " WHERE "
                                "groupName "
                                " IN "
                                "("
                                "SELECT DISTINCT name FROM %2"
                                " WHERE "
                                " category='%3'"
                                ")"
                                ";")
                        .arg(itemsTableName)
                        .arg(groupTableName)
                        .arg(category));
        }
        {
            QSqlQuery q = executeQueryException(
                        QString("DELETE FROM %1"
                                " WHERE "
                                "name='%2'"
                                ";")
                        .arg(categoryTableName)
                        .arg(category));
        }
        {
            QSqlQuery q = executeQueryException(
                        QString("DELETE FROM %1"
                                " WHERE "
                                "category='%2'"
                                ";")
                        .arg(groupTableName)
                        .arg(category));
        }
        {
            QSqlQuery q = executeQueryException("COMMIT;");
        }
    }catch(...)
    {
        QSqlQuery q = executeQueryException("ROLLBACK;");
        throw;
    }
}

void StatisticsConfigDatabase::removeGroup(const QString &category, const QString &group)
{
    {
        QSqlQuery q = executeQueryException("BEGIN TRANSACTION;");
    }
    try
    {
        {
            QSqlQuery q = executeQueryException(
                        QString("DELETE FROM %1"
                                " WHERE "
                                "category='%2'"
                                " AND "
                                "name='%3'"
                                ";")
                        .arg(groupTableName)
                        .arg(category)
                        .arg(group));
        }
        {
            QSqlQuery q = executeQueryException(
                        QString("DELETE FROM %1"
                                " WHERE "
                                "groupName='%2'"
                                ";")
                        .arg(itemsTableName)
                        .arg(group));
        }
        {
            QSqlQuery q = executeQueryException("COMMIT;");
        }
    } catch (...) {
        QSqlQuery q = executeQueryException("ROLLBACK;");
        throw;
    }
}

void StatisticsConfigDatabase::removeItem(const QString &group, const char * const ticker)
{
    assert(ticker);
    assert(strlen(ticker) != 0);
    {
        QSqlQuery q = executeQueryException(
                    QString("DELETE FROM %1"
                            " WHERE "
                            "groupName='%2'"
                            " AND "
                            "ticker='%3'"
                            ";")
                    .arg(itemsTableName)
                    .arg(group)
                    .arg(ticker));
    }
}

StatisticsConfigDatabase::StatisticsConfigDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    db.setDatabaseName(QString(dbName) + ".sqlite");

    if (!db.open()) {
        qDebug() << db.lastError().text();
        throw CantOpenDatabaseException();
    }
    {
        QSqlQuery q = executeQueryException(
                    QString(
                        "create table if not exists "
                        "%1 (name TEXT NOT NULL UNIQUE)"
                        ";")
                    .arg(categoryTableName));
    }
    {
        QSqlQuery q = executeQueryException(
                    QString(
                        "create table if not exists "
                        "%1 (category TEXT NOT NULL, name TEXT NOT NULL UNIQUE)"
                        ";")
                    .arg(groupTableName));
    }
    {
        QSqlQuery q = executeQueryException(
                    QString(
                        "create table if not exists "
                        "%1 (groupName TEXT NOT NULL, ticker TEXT NOT NULL, name TEXT NOT NULL)"
                        ";")
                    .arg(itemsTableName));
    }
}

StatisticsConfigDatabase::~StatisticsConfigDatabase()
{
    db.close();
    db = QSqlDatabase();
    QSqlDatabase::removeDatabase(dbName);
}

StatisticsConfigList StatisticsConfigDatabase::getAll()const
{
    StatisticsConfigList ret;
    for(const auto &catName : getAllCategories())
    {
        StatisticsCathegoryConfig c(catName);
        for(auto const &groupName : getAllGroups(catName))
        {
            StatisticsGroupConfig g(groupName);

            for(const auto &item : getAllItems(groupName))
            {
                g.list.push_back(item);
            }
            c.list.push_back(g);
        }
        ret.push_back(c);
    }
    return ret;
}

bool StatisticsConfigDatabase::isEmpty() const
{
    {
        if(getCategoryCount() > 0)
            return false;
    }
    {
        QSqlQuery q = executeQueryException(
                    QString(
                        "SELECT COUNT(*) FROM "
                        "%1"
                        ";")
                    .arg(groupTableName));

        QSqlRecord rec = q.record();
        if(q.next())
        {
            bool ok;
            int count = q.value(0).toInt(&ok);

            if(!ok)
                throw SqlQueryWrongResultTypeException();

            if(count > 0)
                return false;
        }else
            throw SqlQueryEmptyResultException();
    }
    {
        QSqlQuery q = executeQueryException(
                    QString(
                        "SELECT COUNT(*) FROM "
                        "%1"
                        ";")
                    .arg(itemsTableName));

        QSqlRecord rec = q.record();
        if(q.next())
        {
            bool ok;
            int count = q.value(0).toInt(&ok);

            if(!ok)
                throw SqlQueryWrongResultTypeException();

            if(count > 0)
                return false;
        }else
            throw SqlQueryEmptyResultException();
    }
    return true;
}

size_t StatisticsConfigDatabase::getCategoryCount() const
{
    QSqlQuery q = executeQueryException(
                QString(
                    "SELECT COUNT(*) FROM "
                    "%1"
                    ";")
                .arg(categoryTableName));

    QSqlRecord rec = q.record();
    if(q.next())
    {
        bool ok;
        int count = q.value(0).toInt(&ok);

        if(!ok)
            throw SqlQueryWrongResultTypeException();

        return count;
    }else
        throw SqlQueryEmptyResultException();
}

size_t StatisticsConfigDatabase::getGroupCount(const QString &category) const
{
    QSqlQuery q = executeQueryException(
                QString(
                    "SELECT COUNT(*) FROM "
                    "%1"
                    " WHERE category='%2' "
                    ";")
                .arg(groupTableName)
                .arg(category));

    QSqlRecord rec = q.record();
    if(q.next())
    {
        bool ok;
        int count = q.value(0).toInt(&ok);

        if(!ok)
            throw SqlQueryWrongResultTypeException();

        return count;
    }else
        throw SqlQueryEmptyResultException();
}

size_t StatisticsConfigDatabase::getItemsCount(const QString &category,
                                               const QString &group) const
{
    Q_UNUSED(category);
    QSqlQuery q = executeQueryException(
                QString(
                    "SELECT COUNT(*) FROM "
                    "%1"
                    " WHERE groupName='%2' "
                    ";")
                .arg(itemsTableName)
                .arg(group));

    QSqlRecord rec = q.record();
    if(q.next())
    {
        bool ok;
        int count = q.value(0).toInt(&ok);

        if(!ok)
            throw SqlQueryWrongResultTypeException();

        return count;
    }else
        throw SqlQueryEmptyResultException();
}

QSqlQuery StatisticsConfigDatabase::executeQuery(const QString &query) const
{
    QSqlQuery q = db.exec(query);
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastQuery();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastError();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.record();

    return q;
}

QSqlQuery StatisticsConfigDatabase::executeQueryException(const QString &query) const
{
    QSqlQuery q = db.exec(query);
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastQuery();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.lastError();
    qDebug() << __PRETTY_FUNCTION__ << __LINE__ << q.record();
    if(q.lastError().isValid())
        throw SqlQueryErrorException();

    return q;
}
