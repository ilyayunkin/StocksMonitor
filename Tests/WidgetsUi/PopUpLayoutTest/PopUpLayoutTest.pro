QT += testlib
QT += gui
QT += widgets

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    ../../../src/WidgetsUi/PopUp/PopUpLayout.cpp \
    tst_PopUpLayoutTest.cpp

INCLUDEPATH+=../../../src/
INCLUDEPATH+=../../../include/
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

HEADERS += \
    ../../../src/WidgetsUi/PopUp/PopUpLayout.h
