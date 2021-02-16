include(../../test.pri)
QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_currencyconvertertest.cpp \
    $$ROOT_DIR/src/Application/CurrencyConverter.cpp \
    $$ROOT_DIR/src/Entities/CurrencyCounter.cpp \

HEADERS += \
    $$ROOT_DIR/src/Application/CurrencyConverter.h \
    $$ROOT_DIR/src/Entities/CurrencyCounter.h \
