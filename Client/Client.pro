#-------------------------------------------------
#
# Project created by QtCreator 2014-11-07T20:30:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
        mainwindow.cpp \
    ../common/datastruct.cpp \
    tcpserver.cpp \
    ../common/botanaes256.cpp

HEADERS  += mainwindow.h \
    ../common/datastruct.h \
    tcpserver.h \
    localsocket.h \
    ../common/botanaes256.h

FORMS    += mainwindow.ui

include($$PWD/../common/3rdparty/botan/botan.pri)
