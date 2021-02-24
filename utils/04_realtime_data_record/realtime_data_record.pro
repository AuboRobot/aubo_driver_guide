TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
TARGET = record

HOME_DIR = $$system(env | grep ^HOME= | cut -c 6-)
INCLUDEPATH += $$HOME_DIR/.aubo/include
LIBS += -L$$HOME_DIR/.aubo/lib -laubo_driver -lpthread -ldl

INCLUDEPATH +=
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/trajectory_io.cpp

HEADERS += \
    $$PWD/trajectory_io.h
