#ifndef LIMITSEVENTFILTER_H
#define LIMITSEVENTFILTER_H

#include <QObject>

class QTableView;
class QMenu;
class QAction;
class StocksInterface;

class LimitsEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit LimitsEventFilter(StocksInterface &stocksInterface,
                               QTableView *table);
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    QTableView *table;
    QMenu *menu;
    QAction *portfolioAction;
    QAction *urlAction;
    StocksInterface &stocksInterface;
};

#endif // LIMITSEVENTFILTER_H
