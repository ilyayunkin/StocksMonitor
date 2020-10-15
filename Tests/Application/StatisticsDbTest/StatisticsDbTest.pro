QT += testlib sql
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH+=../../../include/
INCLUDEPATH+=../../../src

SOURCES +=  tst_statisticsdbtest.cpp \
    ../../../src/Application/StatisticsConfigDatabase.cpp \
    ../../../src/Entities/StockId.cpp

HEADERS += \
    ../../../src/Application/StatisticsConfigDatabase.h \
    ../../../src/Entities/StockId.h
