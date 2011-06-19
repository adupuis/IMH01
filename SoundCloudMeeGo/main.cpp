#include "mainwindow.h"

#include <QtGui/QApplication>

//#include <Oauth.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.setOrientation(MainWindow::ScreenOrientationAuto);
//    mainWindow.showFullScreen();
    mainWindow.showExpanded();

    return app.exec();
}
