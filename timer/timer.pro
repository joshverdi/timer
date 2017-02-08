#-------------------------------------------------
#
# Project created by QtCreator 2017-02-04T22:10:30
#
#-------------------------------------------------

QT       += core gui multimedia
CONFIG   += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = timer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsview.cpp \
    ntouchclock.cpp \
    gettimeofday.cpp \
    countdowndialog.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    graphicsview.h \
    ntouchclock.h \
    gettimeofday.h \
    countdowndialog.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    countdowndialog.ui \
    aboutdialog.ui

RESOURCES += \
    timerres.qrc

DISTFILES +=


