#ifndef POPUPLAYOUT_H
#define POPUPLAYOUT_H

#include <QRect>
#include <list>
#include "AbstractPopUpWindow.h"

class PopUpLayout
{
public:
    static PopUpLayout &instance(const QRect availableGeometry);
    void place(AbstractPopUpWindow *w);
    void remove(AbstractPopUpWindow *w);
private:
    PopUpLayout(const QRect availableGeometry);
    QRect availableGeometry;
    std::list<AbstractPopUpWindow *> widgets;

    friend class PopUpLayoutTest;
};

#endif // POPUPLAYOUT_H
