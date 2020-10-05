#ifndef RULESFASADE_H
#define RULESFASADE_H

#include <QByteArrayList>

#include <memory.h>

#include "SourcePluginInterface.h"
#include "ModelsReference.h"
#include "CurrencyCounter.h"

typedef std::vector<std::shared_ptr<SourcePluginInterface>> PluginsList;

class AbstractCurrencyConverter;
class AbstractPocket;
class StocksMonitor;

class RulesFasade
{
    AbstractCurrencyConverter &converter;
    AbstractPocket &portfolio;
    PluginsList &plugins;

    std::vector<std::shared_ptr<StocksMonitor>> monitors;
public:
    RulesFasade(AbstractCurrencyConverter &converter,
                AbstractPocket &portfolio,
                PluginsList &plugins,
                ModelsReferenceList &models);
    ~RulesFasade();
    QString getPortfolioPrice(const char *const currency);
    QString getPortfolioPrice();
    QStringList getAvailibleCurrencies();
};

#endif // RULESFASADE_H
