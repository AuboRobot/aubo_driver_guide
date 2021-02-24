TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HOME_DIR = $$system(env | grep ^HOME= | cut -c 6-)

INCLUDEPATH += $$HOME_DIR/.aubo/include
LIBS += -L$$HOME_DIR/.aubo/lib -laubo_driver -lpthread -ldl

HEADERS += trajectory_io.h

SOURCES += \
    main.cpp \
trajectory_io.cpp
