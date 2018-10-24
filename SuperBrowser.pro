#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T13:38:35
#
#-------------------------------------------------

QT       += core gui webkitwidgets widgets

CONFIG += C++11
QT.testlib.CONFIG -= console

TARGET = SuperSpider
TEMPLATE = app

SOURCES += main.cpp \
    memorycookiejar.cpp \
    superbrowser.cpp \
    webpage.cpp \
    networkaccessmanager.cpp \
    mainwindow.cpp \
    tcpserver.cpp \
    proxydialog.cpp

HEADERS += \
    memorycookiejar.h \
    superbrowser.h \
    webpage.h \
    networkaccessmanager.h \
    mainwindow.h \
    tcpserver.h \
    proxydialog.h

FORMS += \
    proxydialog.ui
