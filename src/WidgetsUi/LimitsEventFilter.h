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

    QTableView *table;
    QMenu *menu;
    QAction *portfolioAction;
    QAction *urlAction;
    StocksInterface &stocksInterface;
public:
    explicit LimitsEventFilter(StocksInterface &stocksInterface,
                               QTableView *table);
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // LIMITSEVENTFILTER_H
