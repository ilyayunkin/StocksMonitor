#ifndef LIMITSEVENTFILTER_H
#define LIMITSEVENTFILTER_H

#include <QObject>

class QTableView;
class QMenu;
class QAction;
class AbstractPocket;
class ModelsReference;

class LimitsEventFilter : public QObject
{
    Q_OBJECT

    QTableView *table;
    QMenu *menu;
    QAction *pocketAction;
    QAction *urlAction;
    ModelsReference &models;
    AbstractPocket &pocket;

    void addLimit(const QModelIndex &index);
public:
    explicit LimitsEventFilter(ModelsReference &models,
                               AbstractPocket &pocket,
                               QTableView *table);
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // LIMITSEVENTFILTER_H
