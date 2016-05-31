#-------------------------------------------------
# Steteskop Project QMAKE File
#-------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#include($$PWD/lib/fft/fft.pri)
#include($$PWD/lib/mcp3208/mcp3208.pri)
include($$PWD/lib/qcustomplot/qcustomplot.pri)

TEMPLATE         = app
TARGET           = steteskop
QT              += core gui printsupport
CONFIG          += c++11 release
DEPENDPATH      += $$PWD
INCLUDEPATH     += $$PWD
MOC_DIR          = $$PWD/build/moc
OBJECTS_DIR      = $$PWD/build/obj
RCC_DIR          = $$PWD/build/qrc
DESTDIR          = $$PWD/build

SOURCES         += $$PWD/main.cpp \
                   $$PWD/mainwidget.cpp 

HEADERS         += $$PWD/mainwidget.h

QMAKE_POST_LINK += mv Makefile build
