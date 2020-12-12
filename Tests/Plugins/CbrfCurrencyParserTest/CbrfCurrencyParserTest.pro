include(../../test.pri)
QT += testlib xml
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_cbrfcurrencyparsertest.cpp \
    ../../../CbrfCurrencyPlugin/CbrfCurrencyParser.cpp

HEADERS += \
    ../../../CbrfCurrencyPlugin/CbrfCurrencyParser.h
