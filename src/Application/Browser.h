#ifndef BROWSER_H
#define BROWSER_H
#include "AbstractBrowser.h"
#include <QDesktopServices>
#include <QUrl>
class Browser : public AbstractBrowser
{
public:
    void openUrl(const Url &url)const{
        QDesktopServices::openUrl(QUrl(toQString(url)));
    }
};

#endif // BROWSER_H
