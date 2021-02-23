TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
TARGET = record
QMAKE_CXXFLAGS += $$system(pkg-config --cflags eigen3)

INCLUDEPATH += /usr/arcs/include
LIBS += -L/usr/arcs/lib -laubo_driver -lpthread
LIBS += $$PWD/libGravity.so $$PWD/lib_Matrix.so

INCLUDEPATH +=
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/trajectory_io.cpp

HEADERS += \
    $$PWD/trajectory_io.h

