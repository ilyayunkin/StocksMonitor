#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QWidget>
#include "AbstractPopUpWindow.h"

class PopUpWindow : public QWidget, public AbstractPopUpWindow
{
    Q_OBJECT
public:
    static void showPopUpWindow(const QString &text, int timeMs = 10000);
    int width()const override{return QWidget::width();}
    int height()const override{return QWidget::height();}
    int x()const override{return QWidget::x();}
    int y()const override{return QWidget::y();}
    void setGeometry(int x, int y, int w, int h) override{QWidget::setGeometry(x, y, w, h);}
private:
    explicit PopUpWindow(const QString &text, int timeMs = 10000);
    ~PopUpWindow();
    void mousePressEvent(QMouseEvent *event);
};

#endif // POPUPWINDOW_H
