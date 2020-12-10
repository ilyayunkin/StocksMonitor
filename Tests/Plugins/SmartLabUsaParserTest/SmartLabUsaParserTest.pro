include(../../test.pri)
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_smartlabusaparsertest.cpp \
    ../../../SmartLabUsaPlugin/SmartLabUsaParser.cpp

HEADERS += \
    ../../../SmartLabUsaPlugin/SmartLabUsaParser.h
