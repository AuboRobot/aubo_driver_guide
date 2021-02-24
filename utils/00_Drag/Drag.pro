TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += $$system(pkg-config --cflags eigen3)
HOME_DIR = $$system(echo $HOME)
INCLUDEPATH += $$HOME_DIR/.aubo/include
LIBS += -L$$HOME_DIR/.aubo/lib -laubo_driver -lpthread -ldl
LIBS += -lpthread
LIBS += $$PWD/libGravity.so $$PWD/lib_Matrix.so $$PWD/libImpendance.so
SOURCES += \
    $$PWD/main.cpp \
    $$PWD/Drag.cpp \
    $$PWD/freedrive.cpp

HEADERS += \
    $$PWD/Drag.h \
    $$PWD/freedrive.h
