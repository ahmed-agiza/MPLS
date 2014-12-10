#-------------------------------------------------
#
# Project created by QtCreator 2014-11-22T15:02:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPLS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    component.cpp \
    instruction.cpp \
    buffer.cpp \
    alu.cpp \
    core.cpp \
    datamemory.cpp \
    exmembuffer.cpp \
    idexbuffer.cpp \
    ifidbuffer.cpp \
    instructionmemory.cpp \
    memwbbuffer.cpp \
    programcounter.cpp \
    register.cpp \
    registerfile.cpp \
    simulator.cpp \
    codeeditor.cpp \
    syntaxhl.cpp \
    registermodel.cpp \
    instructionmodel.cpp \
    memorymodel.cpp \
    buffersmodel.cpp \
    instructionsqueuemodel.cpp

HEADERS  += mainwindow.h \
    component.h \
    instruction.h \
    buffer.h \
    alu.h \
    core.h \
    datamemory.h \
    exmembuffer.h \
    idexbuffer.h \
    ifidbuffer.h \
    instructionmemory.h \
    memwbbuffer.h \
    programcounter.h \
    register.h \
    registerfile.h \
    simulator.h \
    codeeditor.h \
    syntaxhl.h \
    registermodel.h \
    instructionmodel.h \
    memorymodel.h \
    buffersmodel.h \
    instructionsqueuemodel.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    qdarkstyle/style.qrc
