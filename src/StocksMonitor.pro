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
    Application/Application.cpp \
    Application/StocksDatabase.cpp \
    Application/CurrencyConverter.cpp \
    Rules/RulesFasade.cpp \
    Rules/CurrencyCounter.cpp \
    Presenters/LimitBackgrounColor.cpp \
    Presenters/StockHint.cpp \
    Presenters/CurrencyPresenter.cpp \
    Rules/StocksMonitor.cpp \
    WidgetsUi/LimitsEventFilter.cpp \
    WidgetsUi/PocketWidget.cpp \
    WidgetsUi/PopUpWindow.cpp \
    WidgetsUi/Sounds/Signalizer.cpp \
    WidgetsUi/Sounds/SoundDialog.cpp \
    WidgetsUi/StocksEventFilter.cpp \
    WidgetsUi/StoryWidget.cpp \
    WidgetsUi/StocksModelsWidget.cpp \
    WidgetsUi/mainwindow.cpp \
    WidgetsUi/StocksModel.cpp \
    WidgetsUi/StocksLimitsModel.cpp \
    WidgetsUi/PortfolioModel.cpp \
    logger.cpp \
    main.cpp

HEADERS += \
    ../include/AbstractParser.h \
    ../include/SourcePluginInterface.h \
    ../include/ExceptionClasses.h \
    ../include/StocksList.h \
    Application/Application.h \
    Application/StocksDatabase.h \
    Application/CurrencyConverter.h \
    Color.h \
    Rules/AbstractCurrencyConverter.h \
    Rules/AbstractPocket.h \
    Rules/AbstractStocksModel.h \
    Rules/AbstractStocksView.h \
    Rules/StockLimit.h \
    Rules/ModelsReference.h \
    Rules/RulesFasade.h \
    Rules/StocksMonitor.h \
    Rules/CurrencyCounter.h \
    Presenters/LimitBackgrounColor.h \
    Presenters/StockHint.h \
    Presenters/CurrencyPresenter.h \
    WidgetsUi/LimitsEventFilter.h \
    WidgetsUi/Sounds/AbstractSignalizer.h \
    WidgetsUi/Sounds/Signalizer.h \
    WidgetsUi/Sounds/SoundDialog.h \
    WidgetsUi/PocketWidget.h \
    WidgetsUi/StocksEventFilter.h \
    WidgetsUi/StocksModelsWidget.h \
    WidgetsUi/StoryWidget.h \
    WidgetsUi/mainwindow.h \
    WidgetsUi/PopUpWindow.h \
    WidgetsUi/StocksModel.h \
    WidgetsUi/PortfolioModel.h \
    WidgetsUi/StocksLimitsModel.h \
    logger.h

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
