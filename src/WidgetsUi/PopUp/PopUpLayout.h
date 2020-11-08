#ifndef POPUPLAYOUT_H
#define POPUPLAYOUT_H

#include <QWidget>
#include <list>

class PopUpLayout
{
public:
    static PopUpLayout &instance();
    void place(QWidget *w);
    void remove(QWidget *w);
private:
    PopUpLayout();
    std::list<QWidget *> widgets;

    friend class PopUpLayoutTest;
};

#endif // POPUPLAYOUT_H
