include(../../test.pri)
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_smartlabmmvbusaparsertest.cpp \
    ../../../SmartLabMmvbUsaPlugin/SmartLabMmvbUsaParser.cpp

HEADERS += \
    ../../../SmartLabMmvbUsaPlugin/SmartLabMmvbUsaParser.h
