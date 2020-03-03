#-------------------------------------------------
#
# Project created by QtCreator 2017-02-18T21:15:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MineSweeper
TEMPLATE = app


SOURCES += main.cpp\
         cell.cpp \
    field.cpp \
    mainwindow.cpp

HEADERS  += cell.h \
    field.h \
    new_game_form.h \
    help_widget_form.h \
    mainwindow.h

FORMS    += new_game.ui \
    help_widget.ui \
    mainwindow.ui

RESOURCES += \
    Res.qrc

OTHER_FILES +=
