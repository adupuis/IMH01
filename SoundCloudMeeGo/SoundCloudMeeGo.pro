# Add files and directories to ship with the application 
# by adapting the examples below.
# file1.source = myfile
# dir1.source = mydir
DEPLOYMENTFOLDERS = # file1 dir1

symbian:TARGET.UID3 = 0xE5D36151

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += multimedia

QT += network webkit gui declarative

SOURCES += main.cpp \
    mainwindow.cpp \
    user.cpp \
    track.cpp \
    playlist.cpp \
    group.cpp \
    comment.cpp \
    soundcloudapi.cpp \
    app.cpp \
    label.cpp  \
    Oauth.cpp \
    Browser.cpp
#    Playeraudio.cpp


HEADERS += mainwindow.h \
    user.h \
    track.h \
    playlist.h \
    group.h \
    comment.h \
    soundcloudapi.h \
    app.h \
    label.h \
    include/qjson/parser.h \
    Oauth.h \
    Browser.h
#    Playeraudio.h

LIBS += -L../SoundCloudMeeGo/lib -lqjson

FORMS += mainwindow.ui
#QT += declarative

# Please do not modify the following two lines. Required for deployment.
include(deployment.pri)
qtcAddDeployment()

OTHER_FILES += \
    ux-tablet/MainWindow.qml \
    ux-tablet/WindowButton.qml \
    ux-tablet/MenuButton.qml \
    ux-tablet/LoginScreen.qml \
    ux-tablet/Dashboard.qml \
    ux-tablet/Spectrum.qml

