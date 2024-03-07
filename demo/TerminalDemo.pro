QT    += core gui
TARGET = TerminalDemo
TEMPLATE = app
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

OBJECTS_DIR = ../out/obj
MOC_DIR = ../out/generated
UI_DIR = ../out/generated
RCC_DIR = ../out/generated

CONFIG += c++17

SOURCES += \
    DemoDialog.cpp \
    main.cpp

HEADERS += \
    DemoDialog.h 
    
RESOURCES += \
    resource.qrc

include(../pseudoterminal.pri)
