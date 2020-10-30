#ifndef PORTFOLIO_WIDGET_H
#define PORTFOLIO_WIDGET_H

#include <QWidget>
#include "Rules/AbstractCurrencyConverter.h"

namespace Ui {
class PortfolioWidget;
}

class PortfolioModel;
class Application;

class PortfolioWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PortfolioWidget(PortfolioModel *model,
                          Application &rules,
                          QWidget *parent = nullptr);
    ~PortfolioWidget();

private slots:
    void on_clipBoardButton_clicked();
    void on_convertButton_clicked();

private:
    Ui::PortfolioWidget *ui;
    PortfolioModel &model;
    Application &rules;
};

#endif // PORTFOLIO_WIDGET_H
