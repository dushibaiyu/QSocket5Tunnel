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

DEFINES += USE_ENCRYPT #若要启用加密，就取消此宏注释

TEMPLATE = app

CONFIG += C++11

SOURCES += $$PWD/main.cpp \
    $$PWD/tcpserver.cpp \
    $$PWD/threadhandle.cpp \
    $$PWD/clientsocket.cpp \
    $$PWD/../common/datastruct.cpp \
    $$PWD/userconfig.cpp \
    $$PWD/../common/aes.cpp \
    ../common/sshcryptofacility.cpp

HEADERS += \
    $$PWD/../common/datastruct.h \
    $$PWD/tcpserver.h \
    $$PWD/threadhandle.h \
    $$PWD/remotesocket.h \
    $$PWD/clientsocket.h \
    $$PWD/userconfig.h \
    $$PWD/../common/aes.h \
    ../common/sshbotanconversions_p.h \
    ../common/sshcryptofacility_p.h

include (../common/3rdparty/botan/botan.pri)
