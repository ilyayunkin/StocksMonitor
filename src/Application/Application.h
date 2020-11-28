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

class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(QObject *parent = nullptr);
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
    void setDialogs(AbstractDialogs *const dialogs);
    void setStatisticsConfigView(AbstractStatisticsConfigView *configView);
private:

    ViewInterfacesList viewInterfaces;

    std::unique_ptr<PortfolioInterface> portfolioInterface;
    std::vector<std::unique_ptr<StocksMonitor>> monitors;
    std::unique_ptr<StatisticsCsvSaver> csvSaver;
    std::unique_ptr<StatisticsConfigDatabase> statisticsConfigDatabase;
    std::unique_ptr<RulesFasade> rules;
    std::unique_ptr<CurrencyConverter> converter;
    std::vector<std::unique_ptr<BuyRequestDatabase>> buyRequestDatabases;
    std::unique_ptr<PortfolioDatabase> portfolioDatabase;
    std::unique_ptr<StatisticsController> statisticsController;
    std::unique_ptr<ProcessStatisticsController> processStatisticsController;
};

#endif // APPLICATION_H
