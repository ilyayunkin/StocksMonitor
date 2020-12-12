#ifndef APPLICATION_H
#define APPLICATION_H

class SourcePluginInterface;

#include <vector>
#include <memory>

#include <QObject>

#include "PortfolioInterface.h"
#include "Entities/StockId.h"
#include "ViewInterfaces.h"

class RulesFasade;
class CurrencyConverter;
class StocksMonitor;
class BuyRequestDatabase;
class PortfolioDatabase;
class AbstractNotifier;
class AbstractDialogs;
class StatisticsCsvSaver;
class StatisticsConfigDatabase;
class StatisticsController;
class ProcessStatisticsController;
class AbstractStatisticsConfigView;
class Browser;
class CurrencyCourseSource;

class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(const AbstractDialogs &dialogs,
                         QObject *parent = nullptr);
    ~Application();
    ViewInterfacesList &getViewInterfaces(){return viewInterfaces;}
    PortfolioInterface &getPortfolioInterface(){return  *portfolioInterface;}
    StatisticsController &getStatisticsController();
    ProcessStatisticsController &getProcessStatisticsController()
    {return *processStatisticsController;}
    QString getPortfolioPrice(const char *const currency);
    QString getPortfolioPrice();
    QStringList getAvailibleCurrencies();
    QStringList getPluginsList() const;
    StockIdList getStockIdList(const QString &plugin) const;
    void setNotifier(AbstractNotifier *const notifier);
    void setStatisticsConfigView(AbstractStatisticsConfigView *configView);
private:

    ViewInterfacesList viewInterfaces;

    std::unique_ptr<CurrencyCourseSource> currencyCourseSource;
    std::unique_ptr<Browser> browser;
    std::unique_ptr<PortfolioInterface> portfolioInterface;
    std::vector<std::unique_ptr<StocksMonitor>> monitors;
    std::unique_ptr<StatisticsCsvSaver> csvSaver;
    std::unique_ptr<StatisticsConfigDatabase> statisticsConfigDatabase;
    std::unique_ptr<CurrencyConverter> converter;
    std::unique_ptr<RulesFasade> rules;
    std::vector<std::unique_ptr<BuyRequestDatabase>> buyRequestDatabases;
    std::unique_ptr<PortfolioDatabase> portfolioDatabase;
    std::unique_ptr<StatisticsController> statisticsController;
    std::unique_ptr<ProcessStatisticsController> processStatisticsController;
};

#endif // APPLICATION_H
