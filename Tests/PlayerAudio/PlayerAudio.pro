#-------------------------------------------------
#
# Project created by QtCreator 2011-06-18T14:23:35
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = PlayerAudio
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += mobility
MOBILITY = multimedia

TEMPLATE = app

SOURCES += main.cpp \
    ../../SoundCloudMeeGo/Playeraudio.cpp \
    Testapp.cpp

HEADERS += \
    ../../SoundCloudMeeGo/Playeraudio.h \
    Testapp.h
