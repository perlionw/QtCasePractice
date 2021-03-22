#-------------------------------------------------
#
# Project created by QtCreator 2021-03-17T20:49:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nodepad_case
TEMPLATE = app

win32 {
    CONFIG(debug, debug|release) {
        vld_path = "E:\install\Visual Leak Detector"
        !contains(QMAKE_HOST.arch, x86_64) {
            message("x86_32 build")
            LIBS += -L$$vld_path/lib/Win32
        }
        else {
            message("x86_64 build")
            LIBS += -L$$vld_path/lib/Win64
        }

        INCLUDEPATH += $$vld_path/include
        LIBS += -lvld
    }
}


SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    about.h

FORMS    += mainwindow.ui \
    about.ui
