#include "LimitBackgrounColor.h"

#include "Entities/StockLimit.h"

namespace LimitBackgrounColor
{
Color colorForDistance(float d)
{
    if(d <= 0.00)
        return Color::RED;
    if(d < 0.01)
        return Color::ORANGE;
    else if(d < 0.03)
        return Color::YELLOW;
    else if(d < 0.05)
        return Color::GREEN;
    else if(d < 0.08)
        return Color::BLUE;
    else if(d < 0.1)
        return Color::PURPLE;
    else
        return Color::NO_COLOR;
}

Color colorForDistance(const StockLimit &limit)
{
    float d = (limit.price - limit.basePrice) / limit.basePrice;
    return colorForDistance(d);
}

QBrush brushForColor(const Color c)
{
    switch (c) {
    case Color::RED: return QBrush(Qt::GlobalColor::red);
    case Color::ORANGE: return QBrush(QColor::fromRgb(255, 200, 0, 255));
    case Color::YELLOW: return QBrush(Qt::GlobalColor::yellow);
    case Color::GREEN: return QBrush(Qt::GlobalColor::green);
    case Color::BLUE: return QBrush(QColor::fromRgb(0, 200, 255, 255));
    case Color::PURPLE: return QBrush(QColor::fromRgb(149, 117, 205, 255));
    default:
    case Color::NO_COLOR: return QBrush();
    }
}
}
