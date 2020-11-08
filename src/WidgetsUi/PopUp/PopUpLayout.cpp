#include "PopUpLayout.h"
#include <QApplication>
#include <QDesktopWidget>

PopUpLayout &PopUpLayout::instance()
{
    static PopUpLayout instance;
    return instance;
}

void PopUpLayout::place(QWidget *w)
{
    QRect window = QApplication::desktop()->availableGeometry();
    auto y = window.height() - 40 - w->height() + window.y();
    if(!widgets.empty())
    {
        auto lastWidget = widgets.back();
        y = lastWidget->y() - w->height();
    }
    w->setGeometry(window.width() - 40 - w->width() + window.x(),
                y,
                w->width(),
                w->height());
    widgets.push_back(w);
}

void PopUpLayout::remove(QWidget *w)
{
    auto widgetsMoved = std::move(widgets);
    widgetsMoved.remove(w);
    for(auto widget : widgetsMoved)
    {
        place(widget);
    }
}

PopUpLayout::PopUpLayout()
{

}
