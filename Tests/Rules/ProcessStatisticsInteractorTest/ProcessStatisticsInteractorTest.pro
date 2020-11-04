QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_processstatisticsinteractortest.cpp \
    ../../../src/Entities/CurrencyCounter.cpp \
    ../../../src/Entities/Portfolio.cpp \
    ../../../src/Rules/ProcessStatisticsInteractor.cpp

INCLUDEPATH+=../../../src/
INCLUDEPATH+=../../../include/

HEADERS += \
    ../../../src/Entities/CurrencyCounter.h \
    ../../../src/Entities/Portfolio.h \
    ../../../src/Entities/PortfolioEntry.h \
    ../../../src/Entities/Statistics.h \
    ../../../src/Rules/ProcessStatisticsInteractor.h
