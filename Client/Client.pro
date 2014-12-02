#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T20:30:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

DEFINES += USE_ENCRYPT #若要启用加密，就取消此宏注释

CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp \
    ../common/datastruct.cpp \
    tcpserver.cpp \
    ../common/opensslaes.cpp


HEADERS  += mainwindow.h \
    ../common/datastruct.h \
    tcpserver.h \
    localsocket.h \
    ../common/opensslaes.h


FORMS    += mainwindow.ui



win32:
{
win32-g++:{
  LIBS += -LC:/OpenSSL-Win32/lib/MinGW/ -leay32
    PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/MinGW/libeay32.a
}
}

INCLUDEPATH += C:/OpenSSL-Win32/include
DEPENDPATH += C:/OpenSSL-Win32/include



#win32:CONFIG(release, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/static/ -llibeay32MT
#else:win32:CONFIG(debug, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/static/ -llibeay32MTd


#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/VC/static/liblibeay32MT.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/VC/static/liblibeay32MTd.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/VC/static/libeay32MT.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/OpenSSL-Win32/lib/VC/static/libeay32MTd.lib
