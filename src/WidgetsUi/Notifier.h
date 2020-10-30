#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <Rules/AbstractNotifier.h>
#include <QWidget>

class AbstractSignalizer;
class Notifier : public AbstractNotifier
{
public:
    Notifier(QWidget *const alertWidget, AbstractSignalizer *const signalizer);

    // AbstractNotifier interface
    void signalizePortfolio(const QString &name, const float price);
    void signalizeLimit(const QString &name, const float price);
private:
    QWidget *const alertWidget;
    AbstractSignalizer *const signalizer;
};

#endif // NOTIFIER_H
