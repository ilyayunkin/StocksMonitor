#ifndef APPLICATION_H
#define APPLICATION_H

class SourcePluginInterface;

#include <vector>
#include <memory>

#include <QObject>

#include "Rules/PortfolioInterface.h"
#include "Rules/ViewInterfaces.h"

class RulesFasade;
class CurrencyConverter;
class StocksMonitor;
class BuyRequestDatabase;
class PortfolioDatabase;
class AbstractNotifier;
class AbstractDialogs;

class Application : public QObject
{
    Q_OBJECT

    std::vector<std::shared_ptr<StocksMonitor>> monitors;
    RulesFasade *rules;
    CurrencyConverter *converter;
    std::vector<std::shared_ptr<BuyRequestDatabase>> buyRequestDatabases;
    std::shared_ptr<PortfolioDatabase> portfolioDatabase;

public:
    explicit Application(QObject *parent = nullptr);
    ~Application();
    ViewInterfaces &getViewInterfaces();
    QString getPortfolioPrice(const char *const currency);
    QString getPortfolioPrice();
    QStringList getAvailibleCurrencies();
    PortfolioInterface &getPortfolioInterface();
    void setNotifier(AbstractNotifier *const notifier);
    void setDialogs(AbstractDialogs *const dialogs);
};

#endif // APPLICATION_H
