CONFIG(debug, debug|release) {
    DEFINES += _DEBUG
}

VERSION = 1.0.0
TEMPLATE = lib
CONFIG += plugin
QT += core sql dbus webkit
DEFINES += QT_OPENGL_ES_2
DEFINES += OV_VERSION_FACEBOOK=\'\"$$VERSION\"\'
TARGET = FacebookPlugin
INCLUDEPATH += include \
#    ../../../../Data/OVDataFacebookGen/include
INCLUDEPATH += $$SLIMBOX_SDK_PATH/tunerApi/include \
        $$SLIMBOX_SDK_PATH/tunerApi
INCLUDEPATH += $$INSTALLDIR/include
INCLUDEPATH += $$SLIMBOX_SDK_BINARY_PATH/include/freetype2
DEPENDPATH += $$INCLUDEPATH

LIBS += -L$$INSTALLDIR/lib -Wl,-rpath,$$INSTALLDIR/lib \
    -lOVDataLib \
    -lOVDataFacebookGen \
    -lOVDataFacebookLib \
    -lOVSmartParser \
    -lOVSocialBrowser

isEmpty(notshowtext):DEFINES += SHOW_TEXT

# Input

# Install target and resources
target.path = $$INSTALLDIR/resources/plugins/App
resources.files = ./FacebookPlugin.sql
#resources.files = ./FacebookPlugin.sql ./facebookPluginDatabase.db
resources.path = $$INSTALLDIR/resources/plugins/

INSTALLS += target resources

HEADERS += include/OVAppFacebook.h \
    include/OVAppFacebookInterface.h \
    include/OVAppFacebookJob.h \
    include/OVAppFacebookDBHandler.h
SOURCES += src/OVAppFacebook.cpp \
    src/OVAppFacebookInterface.cpp \
    src/OVAppFacebookJob.cpp \
    src/OVAppFacebookDBHandler.cpp
