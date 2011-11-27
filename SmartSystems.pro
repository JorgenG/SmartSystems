#-------------------------------------------------
#
# Project created by QtCreator 2011-11-25T10:49:37
#
#-------------------------------------------------

QT       += core gui

TARGET = SmartSystems
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    roomcontrolwidget.cpp \
    logger/Logger.cpp \
    logger/FileAppender.cpp \
    logger/ConsoleAppender.cpp \
    logger/AbstractStringAppender.cpp \
    logger/AbstractAppender.cpp

HEADERS  += mainwindow.h \
    roomcontrolwidget.h \
    logger/Logger.h \
    logger/FileAppender.h \
    logger/ConsoleAppender.h \
    logger/AbstractStringAppender.h \
    logger/AbstractAppender.h




