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
    CurrencyConverter.cpp \
    CurrencyCounter.cpp \
    WidgetsUi/LimitsEventFilter.cpp \
    WidgetsUi/PocketWidget.cpp \
    WidgetsUi/PopUpWindow.cpp \
    WidgetsUi/Presenters/LimitBackgrounColor.cpp \
    WidgetsUi/Presenters/StockHint.cpp \
    WidgetsUi/Presenters/CurrencyPresenter.cpp \
    WidgetsUi/Sounds/Signalizer.cpp \
    WidgetsUi/Sounds/SoundDialog.cpp \
    WidgetsUi/StocksEventFilter.cpp \
    WidgetsUi/StoryWidget.cpp \
    WidgetsUi/StocksModelsWidget.cpp \
    WidgetsUi/mainwindow.cpp \
    PocketModel.cpp \
    StocksLimitsModel.cpp \
    StocksModel.cpp \
    logger.cpp \
    main.cpp \
    stocksmonitor.cpp

HEADERS += \
    ../include/AbstractParser.h \
    ../include/SourcePluginInterface.h \
    ../include/ExceptionClasses.h \
    ../include/StocksList.h \
    AbstractCurrencyConverter.h \
    AbstractPocket.h \
    Color.h \
    WidgetsUi/LimitsEventFilter.h \
    WidgetsUi/Presenters/LimitBackgrounColor.h \
    WidgetsUi/Presenters/StockHint.h \
    WidgetsUi/Presenters/CurrencyPresenter.h \
    WidgetsUi/Sounds/AbstractSignalizer.h \
    WidgetsUi/Sounds/Signalizer.h \
    WidgetsUi/Sounds/SoundDialog.h \
    WidgetsUi/PocketWidget.h \
    WidgetsUi/StocksEventFilter.h \
    WidgetsUi/StocksModelsWidget.h \
    WidgetsUi/StoryWidget.h \
    WidgetsUi/mainwindow.h \
    WidgetsUi/PopUpWindow.h \
    CurrencyConverter.h \
    CurrencyCounter.h \
    ModelsReference.h \
    PocketModel.h \
    StocksLimitsModel.h \
    StocksModel.h \
    abstractstocksmodel.h \
    logger.h \
    stocksmonitor.h

INCLUDEPATH+=../include/
DESTDIR= ../bin
win32{
RC_ICONS = img/icon.ico
QMAKE_POST_LINK = windeployqt ${DESTDIR_TARGET} ..\bin\plugins\CbrfCurrencyPlugin.dll
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    WidgetsUi/PocketWidget.ui \
    WidgetsUi/Sounds/SoundDialog.ui \
    WidgetsUi/StocksModelsWidget.ui

RESOURCES += \
    res.qrc
