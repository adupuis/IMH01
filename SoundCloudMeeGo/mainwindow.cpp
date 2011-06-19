// checksum 0xa193 version 0x30001
/*
  This file was generated by the Mobile Qt Application wizard of Qt Creator.
  MainWindow is a convenience class containing mobile device specific code
  such as screen orientation handling.
  It is recommended not to modify this file, since newer versions of Qt Creator
  may offer an updated version of it.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QtDeclarative>

#include <Oauth.h>
#include "Playeraudio.h"
#include "soundcloudapi.h"
#include <QGraphicsObject>

MainWindow::MainWindow(QWidget *parent)
{
    Init();
    m_pPlayerAudio = new PlayerAudio(this);
#ifndef FAKE
    m_pSca = new SoundCloudApi();
    QObject::connect(m_pSca, SIGNAL(sigTrackRequestFinished(Track*)),
                     this, SLOT(playTrack(Track*)));
#endif
    QObject::connect(m_pPlayerAudio, SIGNAL(positionChangedRel(QVariant)),
                     rootObject(), SLOT(onUpdateProgress(QVariant)));
    QObject::connect(this, SIGNAL(setWaveForm(QVariant)),
                     rootObject(), SLOT(onSetWaveForm(QVariant)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
     QString contentPath;
#ifdef QT_DEBUG
     contentPath = "/home/qdesert/HACKAT/IMH01/SoundCloudMeeGo";
#else
     contentPath = QApplication::applicationDirPath();
#endif
     setFocusPolicy(Qt::StrongFocus);
     setResizeMode(QDeclarativeView::SizeRootObjectToView);
     setSource(QUrl::fromLocalFile(contentPath + "/ux-tablet/MainWindow.qml"));
     rootContext()->setContextProperty("window", this);
   }

void MainWindow::quit()
{
    qDebug("Mainwindow::quit() called");
    QApplication::quit();
}

void MainWindow::login(QString strLogin, QString strPassword)
{
    qDebug() << "login:" << strLogin;
    qDebug() << "password:" << strPassword;
#ifndef FAKE
    Oauth* oauth = new Oauth;
    oauth->setLogin( strLogin );
    oauth->setPassword( strPassword );
    oauth->start();
    bool ret = connect(oauth, SIGNAL(sigAccessTokenAvailable(QString&)),
                rootObject(), SLOT(spectrumVisible()));
    Q_ASSERT(ret);
    connect(oauth, SIGNAL(sigAccessTokenAvailable(QString&)),
            this, SLOT(getTrackInfo(QString&)));
#else
    QTimer::singleShot(1000, rootObject(), SLOT(spectrumVisible()));
    QTimer::singleShot(1100, this, SLOT(playTrack()));
#endif
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#ifdef Q_OS_SYMBIAN
    showFullScreen();
#elif defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    showMaximized();
#else
    show();
#endif
}


void MainWindow::getTrackInfo(QString& _token)
{
    qDebug() << "getTrackInfo" << _token;
    m_pSca->setOAuthToken(_token);
    m_pSca->getTrack(4951129);
}


void MainWindow::playTrack()
{
    playTrack(0);
}

void MainWindow::playTrack(Track* track)
{
#ifdef FAKE
    m_pPlayerAudio->addFile(QString("/home/meego/local.mp3"));
    m_pPlayerAudio->play();
    emit setWaveForm(QString("/home/meego/local.png"));
#else
    qDebug() << "playTrack" << track->mStreamUrl;
    m_pPlayerAudio->addUrl(track->mStreamUrl);
    m_pPlayerAudio->play();
    emit setWaveForm(QVariant(track->mWaveformUrl));
#endif
}

void MainWindow::playerSeek(int mousex, int width)
{
    m_pPlayerAudio->setPositionRelative( (float)mousex / (float)width );

}
