#include "LimitBackgrounColor.h"

#include "StocksLimitsModel.h"

namespace LimitBackgrounColor
{
Color colorForDistance(float d)
{
    if(d < 0.01)
        return Color::RED;
    if(d < 0.03)
        return Color::ORANGE;
    else if(d < 0.05)
        return Color::YELLOW;
    else if(d < 0.08)
        return Color::GREEN;
    else if(d < 0.1)
        return Color::BLUE;
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
    default:
    case Color::NO_COLOR: return QBrush();
    }
}
}
