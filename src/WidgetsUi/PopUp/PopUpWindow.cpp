#include "PopUpWindow.h"

#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QTimer>
#include <QHBoxLayout>

#include "PopUpLayout.h"

PopUpWindow::PopUpWindow(const QString &text, int timeMs)
{
    QHBoxLayout *lay = new QHBoxLayout(this);
    QLabel *l = new QLabel(text);
    QFont f = l->font();
    f.setPixelSize(16);
    l->setFont(f);

    l->setAlignment(Qt::AlignCenter);
    lay->addWidget(l);

    setWindowFlags(windowFlags()
                   | Qt::Tool
                   | Qt::WindowStaysOnTopHint
                   | Qt::FramelessWindowHint);
    show();
    adjustSize();

    PopUpLayout::instance().place(this);
    {
        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout, this, &QObject::deleteLater);
        t->start(timeMs);
    }
}

PopUpWindow::~PopUpWindow()
{
    PopUpLayout::instance().remove(this);
}

void PopUpWindow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    deleteLater();
}

void PopUpWindow::showPopUpWindow(const QString &text, int timeMs)
{
    new PopUpWindow(text, timeMs);
}
