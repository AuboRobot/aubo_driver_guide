TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/arcs/include
LIBS += -L/usr/arcs/lib -laubo_driver -lpthread

HEADERS += trajectory_io.h

SOURCES += \
    main.cpp \
trajectory_io.cpp
