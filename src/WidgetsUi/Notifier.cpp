#include "Notifier.h"

#include <QApplication>

#include "logger.h"
#include "PopUp/PopUpWindow.h"
#include "Sounds/AbstractSignalizer.h"

Notifier::Notifier(QWidget * const alertWidget, AbstractSignalizer * const signalizer) :
    alertWidget(alertWidget),
    signalizer(signalizer)
{

}

void Notifier::signalizePortfolio(const QString &name, const float price)
{
    QString logMessage = QObject::tr("Portfolio sell price reached:\n"
                                     "%1 %2")
            .arg(name)
            .arg(price);
    Logger::instance().log(logMessage);
    PopUpWindow::showPopUpWindow(logMessage);
    QApplication::alert(alertWidget);
    signalizer->signalize();
}

void Notifier::signalizeLimit(const QString &name, const float price)
{
    QString logMessage = QObject::tr("Stock cheapened:\n%1 %2")
            .arg(name)
            .arg(price);
    Logger::instance().log(logMessage);
    PopUpWindow::showPopUpWindow(logMessage);
    QApplication::alert(alertWidget);
    signalizer->signalize();
}
