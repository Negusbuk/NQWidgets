#-------------------------------------------------
#
# Project created by QtCreator 2013-11-04T11:57:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 4){
  cache()
}

TARGET = NQWidgets
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp \
           flowlayout.cpp \
           nqlogger.cpp \
           nqtokenedit.cpp \
           nqtokenedititem.cpp \
           nqcolorwheel.cpp

HEADERS += mainwindow.h \
           flowlayout.h \
           nqlogger.h \
           nqtokenedit.h \
           nqtokenedititem.h \
           nqcolorwheel.h

OTHER_FILES += stylesheet.qss

RESOURCES += resources.qrc

doc.depends = $(HEADERS)
doc.commands = doxygen documentation/Doxyfile; cd html; git push origin gh-pages; cd .. 
QMAKE_EXTRA_TARGETS += doc
