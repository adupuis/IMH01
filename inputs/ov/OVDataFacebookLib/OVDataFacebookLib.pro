VERSION = 1.1
DEFINES += OV_VERSION=\'\"$$VERSION\"\'

CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

TEMPLATE = lib
QT += core network
TARGET = OVDataFacebookLib

INCLUDEPATH += include \
INCLUDEPATH += $$INSTALLDIR/include

HEADERS += include/OVDataFacebookLib.h \
    include/OVDataFacebookDebug.h

SOURCES += src/OVDataFacebookLib.cpp \

LIBS += -Llib
LIBS +=-L$$INSTALLDIR/lib -Wl,-rpath,$$INSTALLDIR/lib

# Install targets
target.path = $$INSTALLDIR/lib
includes.files = include/*.h
includes.path = $$INSTALLDIR/include/
configfiles.files = OVFacebookConfig.ini
configfiles.path = $$INSTALLDIR

INSTALLS += target configfiles \
   includes
