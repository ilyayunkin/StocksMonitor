#ifndef PortfolioEventFilter_H
#define PortfolioEventFilter_H

#include <QObject>

class QTableView;
class QMenu;
class QAction;
class PortfolioInterface;

class PortfolioEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit PortfolioEventFilter(PortfolioInterface &portfolioInterface,
                               QTableView *table);
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    QTableView *table;
    PortfolioInterface &portfolioInterface;
    QMenu *menu;
    QAction *urlAction;
};

#endif // PortfolioEventFilter_H
