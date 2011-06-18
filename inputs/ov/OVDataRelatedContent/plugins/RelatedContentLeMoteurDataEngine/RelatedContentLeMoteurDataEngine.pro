# #####################################################################
# Build project file for OVData related content library
# Copyright: Orange Vallée
# #####################################################################
VERSION = 1.0.0
DEFINES += OV_VERSION=\'\"$$VERSION\"\'

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

TEMPLATE = lib
CONFIG += plugin
TARGET = ../build/OVDataRelatedContentLeMoteurDataEngine
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
QT = core network

# Input
HEADERS += include/OVDataRelatedContentLeMoteurDataEngine.h \
    include/OVDataRelatedContentLeMoteurDataEngineXmlStreamReader.h
SOURCES += src/OVDataRelatedContentLeMoteurDataEngine.cpp \
    src/OVDataRelatedContentLeMoteurDataEngineXmlStreamReader.cpp

# Install targets
target.path = $$INSTALLDIR/lib
INSTALLS += target

