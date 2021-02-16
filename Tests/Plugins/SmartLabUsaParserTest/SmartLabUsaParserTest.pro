include(../../test.pri)
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_smartlabusaparsertest.cpp \
    $$PLUGINS_DIR/SmartLabUsaPlugin/SmartLabUsaParser.cpp

HEADERS += \
    $$PLUGINS_DIR/SmartLabUsaPlugin/SmartLabUsaParser.h
