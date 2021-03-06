#ifndef LIMITSEVENTFILTER_H
#define LIMITSEVENTFILTER_H

#include <QObject>

class QTableView;
class QMenu;
class QAction;
class BuyRequestInterface;

class LimitsEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit LimitsEventFilter(BuyRequestInterface &buyRequestInterface,
                               QTableView *table);
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    QTableView *table;
    BuyRequestInterface &buyRequestInterface;
    QMenu *menu;
    QAction *portfolioAction;
    QAction *urlAction;
};

#endif // LIMITSEVENTFILTER_H
