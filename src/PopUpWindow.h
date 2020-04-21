#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QWidget>

class PopUpWindow : public QWidget
{
    Q_OBJECT

    PopUpWindow(const QString &text);
public:
    static void showPopUpWindow(const QString &text);
};

#endif // POPUPWINDOW_H
