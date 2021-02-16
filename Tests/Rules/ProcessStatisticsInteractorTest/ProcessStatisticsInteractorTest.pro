include(../../test.pri)

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_processstatisticsinteractortest.cpp \
    $$ROOT_DIR/src/Entities/CurrencyCounter.cpp \
    $$ROOT_DIR/src/Entities/Portfolio.cpp \
    $$ROOT_DIR/src/Rules/ProcessStatisticsInteractor.cpp

HEADERS += \
    $$ROOT_DIR/src/Entities/CurrencyCounter.h \
    $$ROOT_DIR/src/Entities/Portfolio.h \
    $$ROOT_DIR/src/Entities/PortfolioEntry.h \
    $$ROOT_DIR/src/Entities/Statistics.h \
    $$ROOT_DIR/src/Rules/ProcessStatisticsInteractor.h
