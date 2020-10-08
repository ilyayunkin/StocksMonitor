#ifndef NOTIFIER_H
#define NOTIFIER_H

#include <Rules/AbstractNotifier.h>
#include <QWidget>

class AbstractSignalizer;
class Notifier : public AbstractNotifier
{
    QWidget *const alertWidget;
    AbstractSignalizer *const signalizer;

public:
    Notifier(QWidget *const alertWidget, AbstractSignalizer *const signalizer);

    // AbstractNotifier interface
public:
    void signalizePortfolio(const QString &name, const float price);
    void signalizeLimit(const QString &name, const float price);
};

#endif // NOTIFIER_H
