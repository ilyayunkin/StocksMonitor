include(../../test.pri)
QT += testlib xml
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_cbrfmetalparsertest.cpp \
    $$PLUGINS_DIR/CbrfMetalPlugin/CbrfMetalParser.cpp

HEADERS += \
    $$PLUGINS_DIR/CbrfMetalPlugin/CbrfMetalParser.h
