#-------------------------------------------------
#
# Project created by QtCreator 2021-03-17T09:19:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = win_run_case
TEMPLATE = app


CONFIG += c++11

SOURCES += main.cpp\
        widget.cpp \
    mycombobox.cpp

HEADERS  += widget.h \
    mycombobox.h

FORMS    += widget.ui
