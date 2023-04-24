QMAKE_CXXFLAGS += -std=c++11

TARGET = exerciceP10

LIBS += -L../general -lgeneral
PRE_TARGETDEPS += ../general/libgeneral.a

CONFIG-=app_bundle

INCLUDEPATH = ../general

SOURCES += \
    exerciceP10.cc \
    TextViewer.cc

HEADERS += \
    ../general/TextViewer.h