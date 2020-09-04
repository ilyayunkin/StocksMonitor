QT       += core gui network
QT += sql
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PocketModel.cpp \
    PocketWidget.cpp \
    PopUpWindow.cpp \
    SoundDialog.cpp \
    StocksEventFilter.cpp \
    StocksLimitsModel.cpp \
    StocksModel.cpp \
    StocksModelsWidget.cpp \
    StoryWidget.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    stocksmonitor.cpp

HEADERS += \
    ../include/AbstractParser.h \
    ../include/SourcePluginInterface.h \
    ../include/ExceptionClasses.h \
    AbstractPocket.h \
    ModelsReference.h \
    PocketModel.h \
    PocketWidget.h \
    PopUpWindow.h \
    SoundDialog.h \
    StocksEventFilter.h \
    StocksLimitsModel.h \
    ../include/StocksList.h \
    StocksModel.h \
    StocksModelsWidget.h \
    StoryWidget.h \
    abstractstocksmodel.h \
    logger.h \
    mainwindow.h \
    stocksmonitor.h

INCLUDEPATH+=../include/
DESTDIR= ../bin
win32{
RC_ICONS = ../img/icon.ico
QMAKE_POST_LINK = windeployqt ${DESTDIR_TARGET}
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    PocketWidget.ui \
    SoundDialog.ui \
    StocksModelsWidget.ui
