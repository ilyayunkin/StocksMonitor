
INCLUDEPATH+=../../../
INCLUDEPATH+=../../../src/
INCLUDEPATH+=../../../include/

CONFIG += c++17

QMAKE_CXXFLAGS= -O0 --coverage
QMAKE_LFLAGS+= --coverage
