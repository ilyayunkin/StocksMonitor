include(../../benchmark.pri)

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_loadstocksinteractortest.cpp \
    $$ROOT_DIR/src/Entities/CurrencyCounter.cpp \
    $$ROOT_DIR/src/Entities/Entities.cpp \
    $$ROOT_DIR/src/Entities/Portfolio.cpp \
    $$ROOT_DIR/src/Rules/LoadStocksInteractor.cpp \
    $$ROOT_DIR/src/Rules/Subscriptions.cpp

HEADERS += \
    $$ROOT_DIR/include/StocksList.h \
    $$ROOT_DIR/src/Entities/CurrencyCounter.h \
    $$ROOT_DIR/src/Entities/Entities.h \
    $$ROOT_DIR/src/Entities/Portfolio.h \
    $$ROOT_DIR/src/Entities/PortfolioEntry.h \
    $$ROOT_DIR/src/Entities/StockLimit.h \
    $$ROOT_DIR/src/Entities/StocksListHandler.h \
    $$ROOT_DIR/src/Rules/LoadStocksInteractor.h \
    $$ROOT_DIR/src/Rules/Subscriptions.h
