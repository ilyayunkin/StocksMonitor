#ifndef SMARTLABPLUGIN_GLOBAL_H
#define SMARTLABPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SMARTLABPLUGIN_LIBRARY)
#  define SMARTLABPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define SMARTLABPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // SMARTLABPLUGIN_GLOBAL_H
