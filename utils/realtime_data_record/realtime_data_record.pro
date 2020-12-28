TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
TARGET = record

INCLUDEPATH += /usr/arcs/include
LIBS += -L/usr/arcs/lib -laubo_driver -lpthread

INCLUDEPATH +=
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/trajectory_io.cpp

HEADERS += \
    $$PWD/trajectory_io.h
