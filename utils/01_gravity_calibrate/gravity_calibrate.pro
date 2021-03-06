TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
TARGET = gravity_calib

LIBS += $$PWD/lib/libGravity.so $$PWD/lib/libMathAlg.so $$PWD/lib/lib_Matrix.so

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/trajectory_io.cpp

HEADERS += \
    $$PWD/src/trajectory_io.h

QMAKE_CXXFLAGS += $$system(pkg-config --cflags eigen3)
