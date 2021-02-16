include(../../test.pri)
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_SmartLabMmvbParserTest.cpp \
    $$PLUGINS_DIR/SmartLabMmvbPlugin/SmartLabMmvbParser.cpp

HEADERS += \
    $$PLUGINS_DIR/SmartLabMmvbPlugin/SmartLabMmvbParser.h
