#-------------------------------------------------
#
# Project created by QtCreator 2019-04-26T16:46:09
#
#-------------------------------------------------

QT       += core gui

INCLUDEPATH += F:/_programming/QT/QScintilla_gpl-2.11.1/Qt4Qt5
# LIBS += -LF:/_programming/QT/QScintilla_gpl-2.11.1/build-qscintilla-Desktop_Qt_5_12_3_MinGW_64_bit-Release/release/ -lqscintilla2_qt5
LIBS += -LF:/_programming/QT/QScintilla_gpl-2.11.1/build-qscintilla-Desktop_Qt_5_12_3_MinGW_64_bit-Debug/debug/ -lqscintilla2_qt5d

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DatasMaker
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        canclickedqlabel.cpp \
        main.cpp \
        mainwindow.cpp \
        outputviewer.cpp

HEADERS += \
        canclickedqlabel.h \
        mainwindow.h \
        outputviewer.h

FORMS += \
        mainwindow.ui \
        outputviewer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
