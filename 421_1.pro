######################################################################
# Automatically generated by qmake (3.0) ?? 6? 6 21:14:03 2016
######################################################################

TEMPLATE = app
TARGET = 421_1
INCLUDEPATH += .
QT+=widgets
LIBS += -L/usr/local/lib -lpcap
QT       += network
# Input
HEADERS += arpdialog.h \
           etherdialog.h \
           icmpdialog.h \
           ipdialog.h \
           mainwindow.h \
           tcpdialog.h \
           udpdialog.h
FORMS += arpdialog.ui \
         etherdialog.ui \
         icmpdialog.ui \
         ipdialog.ui \
         mainwindow.ui \
         tcpdialog.ui \
         udpdialog.ui
SOURCES += arpdialog.cpp \
           etherdialog.cpp \
           icmpdialog.cpp \
           ipdialog.cpp \
           main.cpp \
           mainwindow.cpp \
           tcpdialog.cpp \
           udpdialog.cpp
