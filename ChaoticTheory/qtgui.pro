QT += core gui opengl
QT += widgets

TEMPLATE = app

SOURCES += \
    src\main.cpp \
    src\mainwindow.cpp \
    src\nglscene.cpp

FORMS += \
    mainwindow.ui

HEADERS += \
    include\mainwindow.h \
    include\nglscene.h

OBJECTS_DIR+=obj

LIBS += -L/$(HOME)/NGL/lib -l NGL
INCLUDEPATH += $$(HOME)/NGL/include/
INCLUDEPATH +=$$PWD/include
CONFIG +=c++11
