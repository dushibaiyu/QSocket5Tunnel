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

SOURCES += main.cpp \
    tcpserver.cpp \
    threadhandle.cpp \
    clientsocket.cpp \
    ../common/datastruct.cpp

HEADERS += \
    ../common/datastruct.h \
    tcpserver.h \
    threadhandle.h \
    remotesocket.h \
    clientsocket.h

