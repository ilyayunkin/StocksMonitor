#ifndef ABSTRACTPOPUPWINDOW_H
#define ABSTRACTPOPUPWINDOW_H

class AbstractPopUpWindow
{
public:
    virtual int width()const = 0;
    virtual int height()const = 0;
    virtual int x()const = 0;
    virtual int y()const = 0;
    virtual void setGeometry(int x, int y, int w, int h) = 0;
};

#endif // ABSTRACTPOPUPWINDOW_H
