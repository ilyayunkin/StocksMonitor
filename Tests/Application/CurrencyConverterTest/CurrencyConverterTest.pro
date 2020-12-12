include(../../test.pri)
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_currencyconvertertest.cpp \
    ../../../src/Application/CurrencyConverter.cpp \
    ../../../src/Entities/CurrencyCounter.cpp \

HEADERS += \
    ../../../src/Application/CurrencyConverter.h \
    ../../../src/Entities/CurrencyCounter.h \
