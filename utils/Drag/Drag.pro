TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += $$system(pkg-config --cflags eigen3)
INCLUDEPATH += /usr/arcs/include
LIBS += -L/usr/arcs/lib -laubo_driver
LIBS += -lpthread
LIBS += $$PWD/libGravity.so $$PWD/lib_Matrix.so $$PWD/libImpendance.so
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/Drag.cpp \
    $$PWD/freedrive.cpp

HEADERS += \
    $$PWD/Drag.h \
    $$PWD/freedrive.h
