include(../../test.pri)

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_loadstocksinteractortest.cpp \
    ../../../src/Entities/CurrencyCounter.cpp \
    ../../../src/Entities/Entities.cpp \
    ../../../src/Entities/Portfolio.cpp \
    ../../../src/Rules/LoadStocksInteractor.cpp \
    ../../../src/Rules/Subscriptions.cpp

HEADERS += \
    ../../../include/StocksList.h \
    ../../../src/Entities/CurrencyCounter.h \
    ../../../src/Entities/Entities.h \
    ../../../src/Entities/Portfolio.h \
    ../../../src/Entities/PortfolioEntry.h \
    ../../../src/Entities/StockLimit.h \
    ../../../src/Entities/StocksListHandler.h \
    ../../../src/Rules/LoadStocksInteractor.h \
    ../../../src/Rules/Subscriptions.h
