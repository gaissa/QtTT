#-------------------------------------------------
#
# Project created by QtCreator 2014-04-07T20:43:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET   = projectfolder
TEMPLATE = app


SOURCES  += main.cpp \
           mainwindow.cpp \
           ./qcustomplot/qcustomplot.cpp \
    filemanager.cpp \
    categorydialog.cpp \
    startdialog.cpp

HEADERS  += mainwindow.h \
            ./qcustomplot/qcustomplot.h \
    filemanager.h \
    categorydialog.h \
    startdialog.h

FORMS    += mainwindow.ui \
    categorydialog.ui \
    startdialog.ui
