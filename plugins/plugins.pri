TEMPLATE = lib

CONFIG += plugin
CONFIG += c++17
QMAKE_CXXFLAGS+= -pedantic-errors
QMAKE_CXXFLAGS+= -Wformat=2
QMAKE_CXXFLAGS+= -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond
QMAKE_CXXFLAGS+= -Wcast-qual -Wcast-align -Wfloat-equal
#QMAKE_CXXFLAGS+= -Wconversion

ROOT_DIR=../../
INCLUDEPATH+=$$ROOT_DIR/include/

DESTDIR= $$ROOT_DIR/bin/plugins

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
