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
    ../common/aes.cpp \
    ../common/sshcryptofacility.cpp

HEADERS  += mainwindow.h \
    ../common/datastruct.h \
    tcpserver.h \
    localsocket.h \
    ../common/aes.h \
    ../common/sshbotanconversions_p.h \
    ../common/sshcryptofacility_p.h


FORMS    += mainwindow.ui

include (../common/3rdparty/botan/botan.pri)
