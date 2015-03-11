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
DEFINES += _WIN32_WINNT=0x0501
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

INCLUDEPATH += $$PWD/QAsioSocket/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QAsioSocket/lib/ -lQAsioSocket
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QAsioSocket/lib/ -lQAsioSocketd
else:unix: LIBS += -L$$PWD/QAsioSocket/lib/ -lQAsioSocket

win32:INCLUDEPATH += C:/OpenSSL-Win32/include
win32:DEPENDPATH += C:/OpenSSL-Win32/include

win32:CONFIG(release, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -llibeay32M
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/OpenSSL-Win32/lib/VC/ -llibeay32Md

win32:INCLUDEPATH += C:/local/boost_1_57_0
win32:DEPENDPATH += C:/local/boost_1_57_0

win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -lboost_date_time-vc100-mt-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -lboost_date_time-vc100-mt-gd-1_57
win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/boost_date_time-vc100-mt-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/boost_date_time-vc100-mt-gd-1_57.lib

win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -lboost_system-vc100-mt-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -lboost_system-vc100-mt-gd-1_57
win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/boost_system-vc100-mt-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/boost_system-vc100-mt-gd-1_57.lib

win32:CONFIG(release, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -lboost_thread-vc100-mt-1_57
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/local/boost_1_57_0/lib32-msvc-10.0/ -lboost_thread-vc100-mt-gd-1_57
win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/boost_thread-vc100-mt-1_57.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += C:/local/boost_1_57_0/lib32-msvc-10.0/boost_thread-vc100-mt-gd-1_57.lib

unix:LIBS += -lcrypto

