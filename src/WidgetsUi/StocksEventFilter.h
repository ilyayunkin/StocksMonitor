#ifndef STOCKSEVENTFILTER_H
#define STOCKSEVENTFILTER_H

#include <QObject>

class QTableView;
class QMenu;
class QAction;
class StocksInterface;
class BuyRequestInterface;

class StocksEventFilter : public QObject
{
    Q_OBJECT

    QTableView *table;
    QMenu *menu;
    QAction *portfolioAction;
    QAction *limitsAction;
    QAction *urlAction;
    StocksInterface &stocksInterface;

    void addLimit(const QModelIndex &index);
public:
    explicit StocksEventFilter(StocksInterface &stocksInterface,
                               QTableView *table);
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // STOCKSEVENTFILTER_H
