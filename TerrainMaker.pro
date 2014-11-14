#-------------------------------------------------
#
# Project created by QtCreator 2014-10-29T16:13:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TerrainMaker
TEMPLATE = app


SOURCES += main.cpp\
        makerdialog.cpp \
    terrain.cpp \
    faultformation.cpp \
    platedrift.cpp \
    terrainmodeling.cpp

HEADERS  += makerdialog.h \
    terrain.h \
    faultformation.h \
    platedrift.h \
    terrainmodeling.h \
    terrainutil.h

FORMS    += makerdialog.ui

CONFIG += c++11

win32: LIBS += -L$$PWD/../../../libraries/log4cpp/qtcreator/build-log4cpp-Desktop_Qt_5_3_MinGW_32bit-Release/release/ -llog4cpp

INCLUDEPATH += $$PWD/../../../libraries/log4cpp/include
DEPENDPATH += $$PWD/../../../libraries/log4cpp/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../libraries/log4cpp/qtcreator/build-log4cpp-Desktop_Qt_5_3_MinGW_32bit-Release/release/log4cpp.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../../libraries/log4cpp/qtcreator/build-log4cpp-Desktop_Qt_5_3_MinGW_32bit-Release/release/liblog4cpp.a
