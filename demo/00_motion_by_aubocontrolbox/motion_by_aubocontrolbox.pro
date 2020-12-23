TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/arcs/include
LIBS += -L/usr/arcs/lib -laubo_driver -lpthread

SOURCES += \
        main.cpp
