#-------------------------------------------------
#
# Project created by QtCreator 2016-05-09T10:59:21
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FCCShow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    fccshowstruct.cpp

HEADERS  += mainwindow.h \
    fccshowstruct.h

FORMS    += mainwindow.ui \
    fccshowstruct.ui

RC_FILE += main.rc

RESOURCES += \
    images.qrc

TRANSLATIONS += cn.ts
