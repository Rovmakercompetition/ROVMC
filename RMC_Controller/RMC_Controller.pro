#-------------------------------------------------
#
# Project created by QtCreator 2017-09-23T11:41:40
#
#-------------------------------------------------

QT       += core gui network gamepad

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RMC_Controller
TEMPLATE = app

@
INCLUDEPATH+=/path/to/libfuzzylite
@

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
