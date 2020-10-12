#ifndef RULESFASADE_H
#define RULESFASADE_H

#include <cstdlib>
#include <memory.h>

#include "SourcePluginInterface.h"
#include "CurrencyCounter.h"
#include "AbstractDialogs.h"
#include "Entities/Entities.h"
#include "AbstractStocksReceiver.h"
#include "ViewInterfaces.h"
#include "PortfolioInterface.h"

class AbstractBuyRequestDatabase;
struct StocksSource
{
    const QString name;
    const std::string currencyCode;
    AbstractBuyRequestDatabase *const db;
};

typedef std::vector<StocksSource> StocksSourceList;
typedef std::vector<AbstractBuyRequestDatabase *> BuyRequestDatabasesList;

class AbstractCurrencyConverter;
class AbstractPortfolioDatabase;
class AbstractNotifier;

class RulesFasade final : public AbstractStocksReceiver
{
    Entities entities;
    ViewInterfaces viewInterfaces;
    PortfolioInterface portfolioInterface;
    BuyRequestDatabasesList buyRequestDatabases;

    AbstractCurrencyConverter *converter = nullptr;
    AbstractPortfolioDatabase *portfolioDb = nullptr;
    AbstractNotifier *notifier = nullptr;
    AbstractDialogs *dialogs = nullptr;

    void updateLimitsStorage(const stocksListHandler handler);
    void updatePortfolioStorage(const stocksListHandler handler);
    void updateStocksView(const stocksListHandler handler);
    void updateLimitsView(const stocksListHandler handler);
    void updatePortfolioView();
    void updatePortfolioView(const size_t row);
    void registerStockSourceInPortfolio(const QString &name, const stocksListHandler handler);
    void signalizePortfolio(const QString &name, const float price);
    void signalizeLimit(const QString &name, const float price);
    float getStockPrice(const stocksListHandler handler, const char *const ticker);
    CurrencyCountersList getPortfolioSum() const;
public:
    RulesFasade();
    ~RulesFasade();

    ViewInterfaces &getViewInterfaces();
    PortfolioInterface &getPortfolioInterface();

    stocksListHandler addStocksSource(const StocksSource source);
    void setConverter(AbstractCurrencyConverter *const converter);
    void setNotifier(AbstractNotifier * const notifier);
    void setPortfolioDatabase(AbstractPortfolioDatabase *const portfolioDb);
    void setDialogs(AbstractDialogs *const dialogs);

    size_t getPortfolioSize() const;
    PortfolioEntry getPortfolioEntry(const size_t i) const;
    Stock getStockForPortfolioEntry(const size_t i) const;
    bool setPortfolioEntryQuantity(size_t row, int quantity);
    bool setPortfolioEntryReferencePrice(size_t row, float referencePrice);
    QString getPortfolioPrice(const char *const currency);
    QString getPortfolioPrice();
    void addToPortfolio(const stocksListHandler handler, const char *const ticker, const int quantity);
    void deletePortfolioEntry(size_t row);
    QStringList getAvailibleCurrencies();

    Stock getStock(const stocksListHandler handler, const size_t i) const;
    Stock getStock(const stocksListHandler handler, const char *const ticker) const;
    size_t getStocksCount(const stocksListHandler handler) const;
    std::string getStocksActualizationTime(const stocksListHandler handler) const;

    StockLimit getStockBuyRequest(const stocksListHandler handler, const size_t i) const;
    StockLimit getStockBuyRequest(const stocksListHandler handler, const char *const ticker) const;
    size_t getStockBuyRequestsCount(const stocksListHandler handler) const;

    void addLimit(const stocksListHandler handler, const char *const ticker, float referencePrice);
    bool setReferencePrice(const stocksListHandler handler, size_t row, float referencePrice);


    // AbstractStocksReceiver interface
public:
    void setStocks(const stocksListHandler handler,
                   StocksList &&stocks,
                   const char *const time) override;
};

#endif // RULESFASADE_H
