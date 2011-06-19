#ifndef TESTAPP_H
#define TESTAPP_H

#include <QDebug>
#include <QObject>
#include "../../SoundCloudMeeGo/Playeraudio.h"

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
        float rel = (float)m_player.position() / (float)m_player.duration();
        qDebug() << "pos" << _p << "rel" << rel ;
        static bool skipped = false;
        if(rel > .5 && !skipped) {
            skipped = true;
            qDebug() << "SKIP";
            //m_player.setPositionRelative(.9);
            m_player.next();
        }
    }

    void slotPlaylist()
    {
        qDebug() << "endofplaylist";
    }

private:
    PlayerAudio m_player;
};
#endif // TESTAPP_H
