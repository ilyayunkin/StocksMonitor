#include "PopUpWindow.h"

#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QHBoxLayout>

PopUpWindow::PopUpWindow(const QString &text)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    QLabel *l = new QLabel(text);
    QFont f = l->font();
    f.setPixelSize(16);
    l->setFont(f);

    l->setAlignment(Qt::AlignCenter);
    lay->addWidget(l);

    setWindowFlags(windowFlags()
                   | Qt::Popup
                   | Qt::WindowStaysOnTopHint);
    show();
    adjustSize();

    QRect window = QApplication::desktop()->availableGeometry();
    setGeometry(window.width() - 40 - width() + window.x(),
                window.height() - 40 - height() + window.y(),
                width(),
                height());

    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &QObject::deleteLater);
    t->start(5000);
}

void PopUpWindow::showPopUpWindow(const QString &text)
{
    new PopUpWindow(text);
}
