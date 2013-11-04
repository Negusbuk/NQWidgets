#-------------------------------------------------
#
# Project created by QtCreator 2013-11-04T11:57:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NQWidgets
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           flowlayout.cpp \
           nqtokenedit.cpp \
           nqtokenedititem.cpp \
           nqcolorwheel.cpp

HEADERS += mainwindow.h \
           flowlayout.h \
           nqtokenedit.h \
           nqtokenedititem.h \
           nqcolorwheel.h

OTHER_FILES += stylesheet.qss

RESOURCES += resources.qrc
