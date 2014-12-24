#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T20:30:09
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = QS5ServerTunnel
CONFIG   += console
CONFIG   -= app_bundle

DEFINES += USE_ENCRYPT #若要启用加密，就取消此宏注释

TEMPLATE = app

CONFIG += C++11


HEADERS += \
    $$PWD/configclass.h \
    $$PWD/serverconmanger.h \
    $$PWD/serverconver.h \
    $$PWD/../common/conversation.h \
    $$PWD/../common/conversationmanger.h \
    $$PWD/../common/datastruct.h \
    $$PWD/../common/opensslaes.h \
    $$PWD/tcpserver.h

SOURCES += \
    $$PWD/configclass.cpp \
    $$PWD/serverconmanger.cpp \
    $$PWD/serverconver.cpp \
    $$PWD/../common/conversation.cpp \
    $$PWD/../common/conversationmanger.cpp \
    $$PWD/../common/opensslaes.cpp \
    $$PWD/main.cpp \
    $$PWD/tcpserver.cpp


win32:LIBS += -LC:/OpenSSL-Win32/lib/MinGW/ -leay32

win32:INCLUDEPATH += C:/OpenSSL-Win32/include
win32:DEPENDPATH += C:/OpenSSL-Win32/include

win32:!win32-g++: PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/MinGW/eay32.lib
else:win32-g++: PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/MinGW/libeay32.a

unix:LIBS += -lcrypto

#include(3rdparty/libev_eventdispatcher.pri)

