#ifndef STOCKSMODELSWIDGET_H
#define STOCKSMODELSWIDGET_H

#include <QWidget>
#include "Rules/StocksInterface.h"
#include "Rules/BuyRequestInterface.h"
#include "Rules/PortfolioInterface.h"

namespace Ui {
class StocksModelsWidget;
}

class StocksModelsWidget : public QWidget
{
    Q_OBJECT
    StocksInterface &stocksInterface;
    BuyRequestInterface &buyRequestInterface;

public:
    explicit StocksModelsWidget(StocksInterface &stocksInterface,
                                BuyRequestInterface &buyRequestInterface,
                                QWidget *parent = nullptr);
    ~StocksModelsWidget();

private:
    Ui::StocksModelsWidget *ui;
};

#endif // STOCKSMODELSWIDGET_H
