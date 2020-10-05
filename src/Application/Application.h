#ifndef APPLICATION_H
#define APPLICATION_H

class SourcePluginInterface;

#include <vector>
#include <memory>

#include <QObject>

#include "Rules/ModelsReference.h"

class RulesFasade;
class Signalizer;
class PortfolioModel;
class CurrencyConverter;

class Application : public QObject
{
    Q_OBJECT

    typedef std::vector<std::shared_ptr<SourcePluginInterface>> PluginsList;

    ModelsReferenceList models;
    Signalizer *signalizer;
    RulesFasade *rules;
    PortfolioModel *portfolio;
    CurrencyConverter *converter;

    PluginsList loadPlugins();
public:
    explicit Application(QObject *parent = nullptr);
    ~Application();
    ModelsReferenceList &modelsReferences();
    QString getPortfolioPrice(const char *const currency);
    QString getPortfolioPrice();
    QStringList getAvailibleCurrencies();
    Signalizer *getSignalizer();
    PortfolioModel *getPortfolio();
};

#endif // APPLICATION_H
