#-------------------------------------------------
#
# Project created by QtCreator 2015-04-14T20:18:08
#
#-------------------------------------------------

QT       += core gui
QT      +=script
QT      +=network
QT += printsupport
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    details.cpp \
    login.cpp \
    data.cpp

HEADERS  += mainwindow.h \
    details.h \
    login.h \
    data.h

FORMS    += mainwindow.ui \
    details.ui \
    login.ui

RESOURCES += \
    img.qrc
RC_FILE += \
    icon.rc
