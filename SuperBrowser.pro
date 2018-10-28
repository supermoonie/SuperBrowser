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
    setextractorsdialog.cpp

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
    setextractorsdialog.h

FORMS += \
    proxydialog.ui \
    interceptordialog.ui \
    serverdialog.ui \
    setextractorsdialog.ui
