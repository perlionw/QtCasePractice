#-------------------------------------------------
#
# Project created by QtCreator 2021-03-17T09:19:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = win_run_case
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

CONFIG += c++11

SOURCES += main.cpp\
        widget.cpp \
    mycombobox.cpp

HEADERS  += widget.h \
    mycombobox.h

FORMS    += widget.ui
