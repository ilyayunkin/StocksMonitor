#ifndef POCKETWIDGET_H
#define POCKETWIDGET_H

#include <QWidget>
#include "Rules/AbstractCurrencyConverter.h"

namespace Ui {
class PocketWidget;
}

class PortfolioModel;
class Application;

class PocketWidget : public QWidget
{
    Q_OBJECT
    PortfolioModel &model;
    Application &rules;
public:
    explicit PocketWidget(PortfolioModel *model,
                          Application &rules,
                          QWidget *parent = nullptr);
    ~PocketWidget();

private slots:
    void on_clipBoardButton_clicked();
    void on_convertButton_clicked();

private:
    Ui::PocketWidget *ui;
};

#endif // POCKETWIDGET_H
