#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T20:30:09
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = Server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG += C++11

SOURCES += $$PWD/main.cpp \
    $$PWD/tcpserver.cpp \
    $$PWD/threadhandle.cpp \
    $$PWD/clientsocket.cpp \
    $$PWD/../common/datastruct.cpp \
    $$PWD/userconfig.cpp \
    $$PWD/../common/botanaes256.cpp

HEADERS += \
    $$PWD/../common/datastruct.h \
    $$PWD/tcpserver.h \
    $$PWD/threadhandle.h \
    $$PWD/remotesocket.h \
    $$PWD/clientsocket.h \
    $$PWD/userconfig.h \
    $$PWD/../common/botanaes256.h

include($$PWD/../common/3rdparty/botan/botan.pri)
