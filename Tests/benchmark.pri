ROOT_DIR=../../../
INCLUDEPATH+=$$ROOT_DIR/
INCLUDEPATH+=$$ROOT_DIR/src/
INCLUDEPATH+=$$ROOT_DIR/include/

QMAKE_CXXFLAGS-= --coverage
QMAKE_LFLAGS-= --coverage
