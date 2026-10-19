TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    asterix.cpp \
    gaulois.cpp

HEADERS += \
    asterix.h \
    gaulois.h \
    marmite.h \
    personnage.h

LIBS += -pthread
