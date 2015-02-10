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

DEFINES += USE_ENCRYPT #若要启用加密，就取消此宏注释

TEMPLATE = app

CONFIG += C++11

SOURCES += $$PWD/main.cpp \
    $$PWD/threadserver.cpp \
    $$PWD/threadhandle.cpp \
    $$PWD/clientsocket.cpp \
    $$PWD/../common/datastruct.cpp \
    $$PWD/userconfig.cpp \
    $$PWD/../common/opensslaes.cpp

HEADERS += \
    $$PWD/../common/datastruct.h \
    $$PWD/threadserver.h \
    $$PWD/threadhandle.h \
    $$PWD/remotesocket.h \
    $$PWD/clientsocket.h \
    $$PWD/userconfig.h \
    $$PWD/../common/opensslaes.h

include ($$PWD/QAsioSocket/src/AsioTcp.pri)


win32:INCLUDEPATH += C:/OpenSSL-Win32/include
win32:DEPENDPATH += C:/OpenSSL-Win32/include

win32:CONFIG(release, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -llibeay32M
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -llibeay32Md

unix:LIBS += -lcrypto

#include(3rdparty/libev_eventdispatcher.pri)



