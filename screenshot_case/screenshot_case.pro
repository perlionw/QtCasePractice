#-------------------------------------------------
#
# Project created by QtCreator 2021-03-22T09:13:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

TARGET = screenshot_case
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
