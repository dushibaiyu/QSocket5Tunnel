#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T20:30:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QS5ClientTunnel
TEMPLATE = app

DEFINES += USE_ENCRYPT #若要启用加密，就取消此宏注释

CONFIG += C++11

SOURCES += \
        $$PWD/main.cpp\
        $$PWD/mainwindow.cpp \
        $$PWD/../common/datastruct.cpp \
        $$PWD/tcpserver.cpp \
        $$PWD/../common/opensslaes.cpp \
        $$PWD/configclass.cpp \
        $$PWD/../common/conversation.cpp \
    clientconver.cpp


HEADERS  += \
        $$PWD/mainwindow.h \
        $$PWD/../common/datastruct.h \
        $$PWD/tcpserver.h \
        $$PWD/../common/opensslaes.h \
        $$PWD/configclass.h \
        $$PWD/../common/conversation.h \
    clientconver.h


FORMS    += $$PWD/mainwindow.ui

RESOURCES += \
    $$PWD/rc.qrc

RC_ICONS = $$PWD/rc/connecd.ico

win32:
{
    win32-g++:{
        LIBS += -LC:/OpenSSL-Win32/lib/MinGW/ -leay32
        PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/MinGW/libeay32.a
    }
}

INCLUDEPATH += C:/OpenSSL-Win32/include
DEPENDPATH += C:/OpenSSL-Win32/include



win32:CONFIG(release, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/static/ -llibeay32MT
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/static/ -llibeay32MTd


win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/VC/static/libeay32MT.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/VC/static/libeay32MTd.lib

