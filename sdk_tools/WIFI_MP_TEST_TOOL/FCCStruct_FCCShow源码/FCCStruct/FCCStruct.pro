#-------------------------------------------------
#
# Project created by QtCreator 2016-05-06T15:06:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FCCStruct
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    frmstructadd.cpp \
    frmstructconfig.cpp

HEADERS  += mainwindow.h \
    frmstructadd.h \
    frmstructconfig.h

FORMS    += mainwindow.ui \
    frmstructadd.ui \
    frmstructconfig.ui

RESOURCES += \
    images.qrc

RC_FILE += main.rc
