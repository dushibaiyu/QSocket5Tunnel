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
    ../common/opensslaes.cpp

HEADERS += \
    $$PWD/../common/datastruct.h \
    $$PWD/tcpserver.h \
    $$PWD/threadhandle.h \
    $$PWD/remotesocket.h \
    $$PWD/clientsocket.h \
    $$PWD/userconfig.h \
    ../common/opensslaes.h


win32:LIBS += -LC:/OpenSSL-Win32/lib/MinGW/ -leay32

win32:INCLUDEPATH += C:/OpenSSL-Win32/include
win32:DEPENDPATH += C:/OpenSSL-Win32/include

win32:!win32-g++: PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/MinGW/eay32.lib
else:win32-g++: PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/MinGW/libeay32.a

unix:LIBS += -lcrypto

#include(3rdparty/libev_eventdispatcher.pri)
