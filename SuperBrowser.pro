#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T13:38:35
#
#-------------------------------------------------

QT += core gui webkitwidgets widgets
QT += websockets

CONFIG += C++11
CONFIG += console

TARGET = SuperSpider
TEMPLATE = app

SOURCES += main.cpp \
    memorycookiejar.cpp \
    superbrowser.cpp \
    webpage.cpp \
    networkaccessmanager.cpp \
    mainwindow.cpp \
    proxydialog.cpp \
    interceptordialog.cpp \
    websocketserver.cpp \
    serverdialog.cpp \
    base64dataviewdialog.cpp \
    extractoreditordialog.cpp

HEADERS += \
    memorycookiejar.h \
    superbrowser.h \
    webpage.h \
    networkaccessmanager.h \
    mainwindow.h \
    proxydialog.h \
    interceptordialog.h \
    websocketserver.h \
    serverdialog.h \
    base64dataviewdialog.h \
    extractoreditordialog.h

FORMS += \
    proxydialog.ui \
    interceptordialog.ui \
    serverdialog.ui \
    base64dataviewdialog.ui \
    extractoreditordialog.ui
