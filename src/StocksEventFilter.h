#ifndef STOCKSEVENTFILTER_H
#define STOCKSEVENTFILTER_H

#include <QObject>

class QTableView;
class QMenu;
class QAction;
class AbstractPocket;
class ModelsReference;

class StocksEventFilter : public QObject
{
    Q_OBJECT

    QTableView *table;
    QMenu *menu;
    QAction *pocketAction;
    QAction *limitsAction;
    ModelsReference &models;
    AbstractPocket &pocket;

    void addLimit(const QModelIndex &index);
public:
    explicit StocksEventFilter(ModelsReference &models,
                               AbstractPocket &pocket,
                               QTableView *table);
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // STOCKSEVENTFILTER_H
