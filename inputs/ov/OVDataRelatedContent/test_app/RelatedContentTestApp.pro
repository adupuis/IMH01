# #####################################################################
# Build project file for OVData related content test app
# Copyright: Orange Vallée
# Author: Arnaud Dupuis (arnaud.dupuis@orange-vallee.net)
# #####################################################################
# Define the app version
VERSION = 1.0.0

if( isEmpty( NO_SLIMBOX ) ){
	QMAKE_LFLAGS = -pg
}
TEMPLATE = app
INCLUDEPATH += ../include ./
SOURCES = main.cpp \
    RCTest.cpp
TARGET = relatedContentTestApp
LIBS += -L../ \
    -lOVDataRelatedContent
DESTDIR = ../out
HEADERS += RCTest.h
FORMS += RCTest.ui
