#include "RulesFasade.h"

#include <algorithm>
#include <set>
#include <assert.h>

#include <QDebug>

#include "AbstractCurrencyConverter.h"
#include "StocksMonitor.h"
#include "ModelsReference.h"
#include "AbstractPocket.h"
#include "Presenters/CurrencyPresenter.h"

RulesFasade::RulesFasade(AbstractCurrencyConverter &converter,
                         AbstractPocket &portfolio,
                         PluginsList &plugins,
                         ModelsReferenceList &models) :
    converter(converter),
    portfolio(portfolio),
    plugins(plugins)
{
    qDebug() << __PRETTY_FUNCTION__;
    for(PluginsList::size_type i = 0; i < plugins.size(); ++i)
    {
        auto &plugin = plugins[i];
        auto &modelsRef = models[i];
        auto monitor = std::make_shared<StocksMonitor>(*(modelsRef.stocksModel.get()),
                                                       plugin->createParser(),
                                                       plugin->getUrl());
        monitors.push_back(monitor);
        QObject::connect(monitor.get(), &StocksMonitor::downloaded,
                         [&models, i](const QByteArray t){models[i].time = t;});
    }
}

RulesFasade::~RulesFasade()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString RulesFasade::getPortfolioPrice(const char * const currency)
{
    return CurrencyPresenter::toText(converter.convert(currency, portfolio.sum()));
}

QString RulesFasade::getPortfolioPrice()
{
    return CurrencyPresenter::toText(portfolio.sum());
}

QStringList RulesFasade::getAvailibleCurrencies()
{
    auto counters = portfolio.sum();

    assert(!counters.list.empty());
    std::set<QByteArray> currencySet;
    for(const auto &c : counters.list)
    {
        currencySet.insert(c.currency);
    }
    QStringList items;
    std::transform(currencySet.begin(), currencySet.end(), std::back_inserter(items),
                   [](const QString &str){return str;});

    return items;
}
