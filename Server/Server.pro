#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T20:30:09
#
#-------------------------------------------------

QT       += core # network

QT       -= gui

TARGET = QS5ServerTunnel
CONFIG   += console
CONFIG   -= app_bundle

win32:DEFINES += _WIN32_WINNT=0x0501

DEFINES += USE_ENCRYPT #若要启用加密，就取消此宏注释
DEFINES += QASIOSOCKET_NOLIB

TEMPLATE = app

CONFIG += C++11

SOURCES += $$PWD/main.cpp \
    $$PWD/clientsocket.cpp \
    $$PWD/../common/datastruct.cpp \
    $$PWD/userconfig.cpp \
    server.cpp

HEADERS += \
    $$PWD/../common/datastruct.h \
    $$PWD/remotesocket.h \
    $$PWD/clientsocket.h \
    $$PWD/userconfig.h \
    server.h

include ($$PWD/../../QAsioSocket/QAsioSocket.pri)
include ($$PWD/../../QAes/QAes/QAes.pri)



