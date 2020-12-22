include(../../test.pri)
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_cbrfmetalcachedbtest.cpp \
    ../../../CbrfMetalPlugin/CbrfMetalCacheDb.cpp

HEADERS += \
    ../../../CbrfMetalPlugin/CbrfMetalCacheDb.h
