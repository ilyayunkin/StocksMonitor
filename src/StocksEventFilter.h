#ifndef STOCKSEVENTFILTER_H
#define STOCKSEVENTFILTER_H

#include <QObject>

class QTableView;
class QMenu;
class QAction;
class AbstractPocket;

class StocksEventFilter : public QObject
{
    Q_OBJECT

    QTableView *table;
    QMenu *menu;
    QAction *pocketAction;
    QString plugin;
    AbstractPocket &pocket;
public:
    explicit StocksEventFilter(QString plugin, AbstractPocket &pocket, QTableView *table);
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // STOCKSEVENTFILTER_H
