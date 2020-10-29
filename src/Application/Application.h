#ifndef APPLICATION_H
#define APPLICATION_H

class SourcePluginInterface;

#include <vector>
#include <memory>

#include <QObject>

#include "Rules/PortfolioInterface.h"
#include "Rules/ViewInterfaces.h"
#include "Entities/StockId.h"

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
class AbstractStatisticsConfigView;

class Application : public QObject
{
    Q_OBJECT

    std::vector<std::shared_ptr<StocksMonitor>> monitors;
    std::shared_ptr<RulesFasade> rules;
    std::shared_ptr<CurrencyConverter> converter;
    std::vector<std::shared_ptr<BuyRequestDatabase>> buyRequestDatabases;
    std::shared_ptr<PortfolioDatabase> portfolioDatabase;
    std::shared_ptr<StatisticsCsvSaver> csvSaver;
    std::shared_ptr<StatisticsConfigDatabase> statisticsConfigDatabase;
    std::shared_ptr<StatisticsController> statisticsController;

public:
    explicit Application(QObject *parent = nullptr);
    ~Application();
    ViewInterfaces &getViewInterfaces();
    PortfolioInterface &getPortfolioInterface();
    StatisticsController &getStatisticsController();
    QString getPortfolioPrice(const char *const currency);
    QString getPortfolioPrice();
    QStringList getAvailibleCurrencies();
    QStringList getPluginsList() const;
    StockIdList getStockIdList(const QString &plugin) const;
    void setNotifier(AbstractNotifier *const notifier);
    void setDialogs(AbstractDialogs *const dialogs);
    void setStatisticsConfigView(AbstractStatisticsConfigView *configView);
};

#endif // APPLICATION_H
