#ifndef ABSTRACTBROWSER_H
#define ABSTRACTBROWSER_H

#include "StringTypes.h"

class AbstractBrowser
{
public:
    virtual void openUrl(const Url &url)const = 0;
};

#endif // ABSTRACTBROWSER_H
