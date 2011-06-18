#ifndef TESTAPP_H
#define TESTAPP_H

#include <QDebug>
#include <QObject>
#include "Playeraudio.h"

class TestApp : public QObject
{
    Q_OBJECT
public :
    TestApp();

    virtual ~TestApp();

    void load() {
        qDebug() << "loading";
        m_player.addUrl("http://api.soundcloud.com/tracks/13158665/stream?client_id=ef7c3301f5a463034354f0bfa1ee0236");
        m_player.addUrl("http://api.soundcloud.com/tracks/4951129/stream?client_id=ef7c3301f5a463034354f0bfa1ee0236");
    }

    void play() {
        qDebug() << "play";
        m_player.play();
    }

private slots:
    void slotTrack() {
        qDebug() << "endoftrack";
    }
    void slotPos(qint64 _p) {
        qDebug() << "pos" << _p << "rel" << (float)m_player.position() / (float)m_player.duration();
    }

    void slotPlaylist()
    {
        qDebug() << "endofplaylist";
    }

private:
    PlayerAudio m_player;
};
#endif // TESTAPP_H
