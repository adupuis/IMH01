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

LIBS += -L../../Tools/OVDebug -lOVDebug -Wl,--rpath -Wl,../../Tools/OVDebug

TARGET = OVDataRelatedContent
INCLUDEPATH += ../../Tools/OVDebug/include
INCLUDEPATH += $$OBJECTS_DIR \
    . \
    include \
    ../OVDataLib/include
INCLUDEPATH += $$INSTALLDIR/include
DEPENDPATH += $$INCLUDEPATH
QT = core gui

# Input
HEADERS += include/OVDataRelatedContent.h \
    include/OVDataRelatedContentResult.h \
    include/OVDataRelatedContentDataEngineInterface.h
SOURCES += src/OVDataRelatedContent.cpp \
    src/OVDataRelatedContentResult.cpp

# Install targets
target.path = $$INSTALLDIR/lib
includes.files = include/*.h
includes.path = $$INSTALLDIR/include/

# i18n.files = i18n/*.qm
# i18n.path = $$BASE_DATADIR/i18n
# data.files = data/*
# data.path = $$BASE_DATADIR
# images.files = images/*
# images.path = $$BASE_DATADIR/images
INSTALLS += target \
    includes
