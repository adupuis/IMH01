# #####################################################################
# Build project file for OVData related content test plugin for data engine plugin system
# Copyright: Orange Vallée
# Author: Arnaud Dupuis (arnaud.dupuis@orange-vallee.net)
# #####################################################################
# Define the library version
VERSION = 1.0.0

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

if( isEmpty( NO_SLIMBOX ) ){
	QMAKE_LFLAGS += -pg
}
TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += ./ \
    include \
    ../../include
HEADERS = include/RelatedContentTestPlugin.h
SOURCES = src/RelatedContentTestPlugin.cpp
TARGET = $$qtLibraryTarget(RelatedContentTestPlugin)
DESTDIR = ../build

# install
target.path = $${INSTALLDIR}/lib/OVRelatedContentPlugins
INSTALLS += target
