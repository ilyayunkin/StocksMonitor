#ifndef LIMITBACKGROUNCOLOR_H
#define LIMITBACKGROUNCOLOR_H

#include <QBrush>
#include "Color.h"

class StockLimit;

namespace LimitBackgrounColor
{
Color colorForDistance(float d);

Color colorForDistance(const StockLimit &limit);

QBrush brushForColor(const Color c);
};

#endif // LIMITBACKGROUNCOLOR_H
