TEMPLATE = app

QT       += core gui

TARGET = osk

LIBS += User32.lib

CONFIG(debug) {
    LIBS += ../jansson/Debug/jansson.lib
} else {
    LIBS += ../jansson/Release/jansson.lib
}

INCLUDEPATH += ../jansson/jansson/src ../jansson

SOURCES += main.cpp\
    onscreenkeyboard.cpp \
    onscreenbutton.cpp \
    application.cpp

HEADERS  += \
    onscreenkeyboard.h \
    onscreenbutton.h \
    application.h
