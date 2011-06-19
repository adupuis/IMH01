#include "mainwindow.h"

#include <QtGui/QApplication>

#include "Playeraudio.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
//    mainWindow.showFullScreen();
    mainWindow.showExpanded();

    PlayerAudio player;

    player.addUrl("http://api.soundcloud.com/tracks/4951129/stream?client_id=ef7c3301f5a463034354f0bfa1ee0236");

    QObject::connect(&player, SIGNAL(positionChangedRel(QVariant)),
                     mainWindow.rootObject(), SLOT(onUpdateProgress(QVariant)));
    player.play();

    return app.exec();
}
