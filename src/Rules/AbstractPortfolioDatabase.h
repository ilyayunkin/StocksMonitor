#ifndef ABSTRACTPORTFOLIDATABASE_H
#define ABSTRACTPORTFOLIDATABASE_H

#include "Entities/PortfolioEntry.h"

class AbstractPortfolioDatabase
{
public:
    virtual ~AbstractPortfolioDatabase() = default;
    virtual void add(const PluginName &plugin, const char *const ticker, int quantity) = 0;
    virtual void update(const PortfolioEntry &entry) = 0;
    virtual void deleteEntry(const char *const ticker) = 0;
    virtual PortfolioEntryList getAll() = 0;
};


#endif // ABSTRACTPORTFOLIDATABASE_H
