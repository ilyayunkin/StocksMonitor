#ifndef STOCKSMODELSWIDGET_H
#define STOCKSMODELSWIDGET_H

#include <QWidget>

namespace Ui {
class StocksModelsWidget;
}

class ModelsReference;

class AbstractPocket;

class StocksModelsWidget : public QWidget
{
    Q_OBJECT
    ModelsReference &models;

public:
    explicit StocksModelsWidget(ModelsReference &models, AbstractPocket &pocket, QWidget *parent = nullptr);
    ~StocksModelsWidget();

private:
    Ui::StocksModelsWidget *ui;
};

#endif // STOCKSMODELSWIDGET_H
