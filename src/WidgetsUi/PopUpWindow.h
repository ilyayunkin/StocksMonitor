#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QWidget>

class PopUpWindow : public QWidget
{
    Q_OBJECT
public:
    static void showPopUpWindow(const QString &text, int timeMs = 10000);
private:
    explicit PopUpWindow(const QString &text, int timeMs = 10000);
    void mousePressEvent(QMouseEvent *event);
};

#endif // POPUPWINDOW_H
