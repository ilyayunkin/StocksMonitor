include(../../test.pri)
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_SmartLabMmvbParserTest.cpp \
    ../../../SmartLabMmvbPlugin/SmartLabMmvbParser.cpp

HEADERS += \
    ../../../SmartLabMmvbPlugin/SmartLabMmvbParser.h
