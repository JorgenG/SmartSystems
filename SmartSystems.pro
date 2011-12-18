#-------------------------------------------------
#
# Project created by QtCreator 2011-11-25T10:49:37
#
#-------------------------------------------------

QT       += core gui network xml
LIBS += D:\\Workspace\\SmartSystems\\nidaqmx\\NIDAQmx.lib

TARGET = SmartSystems
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
        roomcontrolwidget.cpp \
        server/server.cpp \
        server/connection.cpp \
        shareddata/shareddata.cpp \
        shareddata/roomdata.cpp \
        shareddata/newoutputhandler.cpp \
    logger.cpp \
    nidaqmx/niinterface.cpp \
    automodethread.cpp

HEADERS += mainwindow.h \
        roomcontrolwidget.h \
        server/server.h \
        server/connection.h \
        shareddata/shareddata.h \
        shareddata/roomdata.h \
        shareddata/newoutputhandler.h \
    logger.h \
    nidaqmx/NIDAQmx.h \
    nidaqmx/niinterface.h \
    automodethread.h



















