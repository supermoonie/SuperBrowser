#-------------------------------------------------
#
# Project created by QtCreator 2018-09-14T13:38:35
#
#-------------------------------------------------

QT       += network webkitwidgets


TARGET = SuperSpider
CONFIG   += console
CONFIG   += C++11
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = ../bin


SOURCES += main.cpp \
    terminal.cpp \
    encoding.cpp \
    memorycookiejar.cpp \
    superbrowser.cpp \
    webpage.cpp \
    receivethread.cpp \
    networkaccessmanager.cpp

HEADERS += \
    terminal.h \
    encoding.h \
    memorycookiejar.h \
    superbrowser.h \
    webpage.h \
    receivethread.h \
    networkaccessmanager.h
