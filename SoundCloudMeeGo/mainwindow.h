// checksum 0x9a77 version 0x30001
/*
  This file was generated by the Mobile Qt Application wizard of Qt Creator.
  MainWindow is a convenience class containing mobile device specific code
  such as screen orientation handling.
  It is recommended not to modify this file, since newer versions of Qt Creator
  may offer an updated version of it.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QDeclarativeView>

class PlayerAudio;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QDeclarativeView
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    void Init();
    Q_INVOKABLE void quit();
    Q_INVOKABLE void login(QString strLogin, QString strPassword);

    void setOrientation(ScreenOrientation orientation);
    void showExpanded();
private slots:
    void playTrack();
private:
    Ui::MainWindow *ui;
    PlayerAudio*     m_pPlayerAudio;
};

#endif // MAINWINDOW_H
