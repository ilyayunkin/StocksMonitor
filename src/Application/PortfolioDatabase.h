#ifndef PORTFOLIODATABASE_H
#define PORTFOLIODATABASE_H

#include "Rules/AbstractPortfolioDatabase.h"
#include <QtSql>

class PortfolioDatabase final: public AbstractPortfolioDatabase
{
    QSqlDatabase db;
    QSqlQuery executeQuery(const QString &query);
public:
    PortfolioDatabase();

    // AbstractPortfolioDatabase interface
public:
    void add(const QString &plugin, const char *const ticker, int quantity) override;
    void update(const PortfolioEntry &entry) override;
    void deleteEntry(const char *const ticker) override;
    PortfolioEntryList getAll() override;
};

#endif // PORTFOLIODATABASE_H
