#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T20:30:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QS5ClientTunnel
TEMPLATE = app

DEFINES += USE_ENCRYPT QASIOSOCKET_NOLIB #若要启用加密，就取消此宏注释

CONFIG += C++11

SOURCES += \
        $$PWD/main.cpp\
        $$PWD/mainwindow.cpp \
        $$PWD/configclass.cpp


HEADERS  += \
        $$PWD/mainwindow.h \
        $$PWD/localsocket.h \
        $$PWD/configclass.h


FORMS    += $$PWD/mainwindow.ui

RESOURCES += \
    $$PWD/rc.qrc

RC_ICONS = $$PWD/rc/connecd.ico


include ($$PWD/../../QAsioSocket/QAsioSocket.pri)
