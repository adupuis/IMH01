# #####################################################################
# Build project file for OVData related content library
# Copyright: Orange Vallée
# Author: Arnaud Dupuis (arnaud.dupuis@orange-vallee.net)
# #####################################################################
VERSION = 1.0.0
DEFINES += OV_VERSION=\'\"$$VERSION\"\'

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

TEMPLATE = lib
CONFIG += plugin
TARGET = OVDataRelatedContentYouTubeDataEngine
OBJECTS_DIR = build
UI_DIR = $$OBJECTS_DIR
MOC_DIR = $$OBJECTS_DIR
RCC_DIR = $$OBJECTS_DIR

INCLUDEPATH += ../../../../Tools/OVDebug/include
INCLUDEPATH += $$OBJECTS_DIR
INCLUDEPATH += .
INCLUDEPATH += include
INCLUDEPATH += ../../include

DEPENDPATH += $$INCLUDEPATH

LIBS += -L../../../../Tools/OVDebug -lOVDebug -Wl,--rpath -Wl,../../../../Tools/OVDebug

QT = core network gui

# Input
HEADERS += include/OVDataRelatedContentYouTubeDataEngine.h \
    include/OVDataRelatedContentYouTubeDataEngineXmlStreamReader.h
SOURCES += src/OVDataRelatedContentYouTubeDataEngine.cpp \
    src/OVDataRelatedContentYouTubeDataEngineXmlStreamReader.cpp

# Install targets
target.path = $$INSTALLDIR/lib
INSTALLS += target
