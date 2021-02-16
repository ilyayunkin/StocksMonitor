include(../../test.pri)
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_smartlabmmvbusaparsertest.cpp \
    $$PLUGINS_DIR/SmartLabMmvbUsaPlugin/SmartLabMmvbUsaParser.cpp

HEADERS += \
    $$PLUGINS_DIR/SmartLabMmvbUsaPlugin/SmartLabMmvbUsaParser.h
