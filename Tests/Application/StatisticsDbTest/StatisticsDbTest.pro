include(../../test.pri)

QT += testlib sql
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_statisticsdbtest.cpp \
    $$ROOT_DIR/src/Application/StatisticsConfigDatabase.cpp \
    $$ROOT_DIR/src/Entities/StockId.cpp

HEADERS += \
    $$ROOT_DIR/src/Application/StatisticsConfigDatabase.h \
    $$ROOT_DIR/src/Entities/StockId.h
