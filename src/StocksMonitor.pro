QT       += core gui network
QT += sql
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

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
    Application/BuyRequestDatabase.cpp \
    Application/PortfolioDatabase.cpp \
    Application/CurrencyConverter.cpp \
    Application/StatisticsConfigDatabase.cpp \
    Application/StatisticsCsvSaver.cpp \
    Application/StocksMonitor.cpp \
    Application/Controllers/StatisticsController.cpp \
    Application/PortfolioInterface.cpp \
    Application/StocksInterface.cpp \
    Application/BuyRequestInterface.cpp \
    Entities/Entities.cpp \
    Entities/StockId.cpp \
    Entities/CurrencyCounter.cpp \
    Rules/EditBuyRequestInteractor.cpp \
    Rules/EditPortfolioInteractor.cpp \
    Rules/LoadStocksInteractor.cpp \
    Rules/RulesFasade.cpp \
    Presenters/LimitBackgrounColor.cpp \
    Presenters/StockHint.cpp \
    Presenters/CurrencyPresenter.cpp \
    Rules/StatisticsInteractor.cpp \
    Rules/Subscriptions.cpp \
    WidgetsUi/Dialogs.cpp \
    WidgetsUi/LimitsEventFilter.cpp \
    WidgetsUi/Notifier.cpp \
    WidgetsUi/PopUpWindow.cpp \
    WidgetsUi/PortfolioWidget.cpp \
    WidgetsUi/Sounds/Signalizer.cpp \
    WidgetsUi/Sounds/SoundDialog.cpp \
    WidgetsUi/StatisticsModel.cpp \
    WidgetsUi/StatisticsWidget.cpp \
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
    Application/BuyRequestDatabase.h \
    Application/Controllers/AbstractStatisticsConfigView.h \
    Application/Controllers/StatisticsConfigIndex.h \
    Application/PortfolioDatabase.h \
    Application/CurrencyConverter.h \
    Application/StatisticsConfigDatabase.h \
    Application/StatisticsCsvSaver.h \
    Application/StocksMonitor.h \
    Application/Controllers/StatisticsController.h \
    Application/ViewInterfaces.h \
    Application/BuyRequestInterface.h \
    Application/PortfolioInterface.h \
    Application/StocksInterface.h \
    Color.h \
    Entities/Entities.h \
    Entities/PortfolioEntry.h \
    Entities/Statistics.h \
    Entities/StocksListHandler.h \
    Entities/StockLimit.h \
    Entities/StockId.h \
    Entities/CurrencyCounter.h \
    Entities/AbstractStatisticsView.h \
    Rules/AbstractDialogs.h \
    Rules/AbstractNotifier.h \
    Rules/AbstractStatisticsConfigDatabase.h \
    Rules/EditBuyRequestInteractor.h \
    Rules/EditPortfolioInteractor.h \
    Rules/LoadStocksInteractor.h \
    Rules/StatisticsInteractor.h \
    Rules/StatisticsResults.h \
    Rules/Subscriptions.h \
    Rules/AbstractBuyRequestDatabase.h \
    Rules/AbstractCurrencyConverter.h \
    Rules/AbstractPortfolioDatabase.h \
    Rules/AbstractStocksReceiver.h \
    Rules/AbstractStocksView.h \
    Rules/RulesFasade.h \
    Presenters/LimitBackgrounColor.h \
    Presenters/StockHint.h \
    Presenters/CurrencyPresenter.h \
    WidgetsUi/Dialogs.h \
    WidgetsUi/LimitsEventFilter.h \
    WidgetsUi/Notifier.h \
    WidgetsUi/PortfolioWidget.h \
    WidgetsUi/Sounds/AbstractSignalizer.h \
    WidgetsUi/Sounds/Signalizer.h \
    WidgetsUi/Sounds/SoundDialog.h \
    WidgetsUi/StatisticsModel.h \
    WidgetsUi/StatisticsWidget.h \
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
    WidgetsUi/PortfolioWidget.ui \
    WidgetsUi/Sounds/SoundDialog.ui \
    WidgetsUi/StatisticsWidget.ui \
    WidgetsUi/StocksModelsWidget.ui

RESOURCES += \
    res.qrc
