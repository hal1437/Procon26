#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T16:20:45
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeveloperGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../Structure/Block.cpp \
    ../../Structure/Field.cpp \
    ../../Structure/Heuristic.cpp \
    ../../Structure/test.cpp \
    FieldMap.cpp \
    CreateBlockDialog.cpp

HEADERS  += mainwindow.h \
    ../../Structure/Block.h \
    ../../Structure/Field.h \
    ../../Structure/Heuristic.h \
    ../../Structure/Multi.hpp \
    ../../Structure/Point.hpp \
    FieldMap.h \
    ../../Utility/CLOCKWISE_FOR.hpp \
    ../../Utility/Constants.hpp \
    ../../Utility/SYNTHESIZE.hpp \
    CreateBlockDialog.h

FORMS    += mainwindow.ui \
    CreateBlockDialog_UI.ui
