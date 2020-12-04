#include "PopUpLayout.h"

PopUpLayout &PopUpLayout::instance(const QRect availableGeometry)
{
    static PopUpLayout instance(availableGeometry);
    return instance;
}

void PopUpLayout::place(AbstractPopUpWindow *w)
{
    auto y = availableGeometry.height() - 40 - w->height() + availableGeometry.y();
    if(!widgets.empty())
    {
        auto lastWidget = widgets.back();
        y = lastWidget->y() - w->height();
    }
    const auto x = availableGeometry.width() - 40 - w->width() + availableGeometry.x();
    w->setGeometry(x, y, w->width(), w->height());
    widgets.push_back(w);
}

void PopUpLayout::remove(AbstractPopUpWindow *w)
{
    auto widgetsMoved = std::move(widgets);
    widgetsMoved.remove(w);
    for(auto widget : widgetsMoved)
    {
        place(widget);
    }
}

PopUpLayout::PopUpLayout(const QRect availableGeometry)
    : availableGeometry(availableGeometry)
{

}
