#-------------------------------------------------
#
# Project created by QtCreator 2011-11-25T10:49:37
#
#-------------------------------------------------

QT       += core gui network

TARGET = SmartSystems
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        roomcontrolwidget.cpp \
        shareddata/shareddata.cpp \
        shareddata/roomdata.cpp \
        shareddata/newoutputhandler.cpp
        server/server.cpp \
        server/connection.cpp

HEADERS += mainwindow.h \
        roomcontrolwidget.h \
        shareddata/shareddata.h \
        shareddata/roomdata.h \
        shareddata/newoutputhandler.h \
        server/server.h \
        server/connection.h
