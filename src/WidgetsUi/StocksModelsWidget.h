#ifndef STOCKSMODELSWIDGET_H
#define STOCKSMODELSWIDGET_H

#include <QWidget>
#include "Application/ViewInterfaces.h"
#include "Application/PortfolioInterface.h"

namespace Ui {
class StocksModelsWidget;
}

class StocksModelsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StocksModelsWidget(ViewInterfacesPair &viewInterfaces,
                                QWidget *parent = nullptr);
    ~StocksModelsWidget();

private:
    Ui::StocksModelsWidget *ui;
    ViewInterfacesPair &viewInterfaces;
    StocksInterface &stocksInterface;
    BuyRequestInterface &buyRequestInterface;
};

#endif // STOCKSMODELSWIDGET_H
