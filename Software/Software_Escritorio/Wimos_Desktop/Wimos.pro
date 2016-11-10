#-------------------------------------------------
#
# Project created by QtCreator 2015-10-27T19:02:57
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wimos
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lib/qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.h \
    lib/qcustomplot/qcustomplot.h

FORMS    += mainwindow.ui
