QT       += core gui widgets
CONFIG   += c++20
TARGET    = Potion_Interblocage
TEMPLATE  = app

msvc: QMAKE_CXXFLAGS += /utf-8

SOURCES += main.cpp \
    asterix.cpp \
    gaulois.cpp
HEADERS += \
    asterix.h \
    gaulois.h \
    marmite.h \
    personnage.h