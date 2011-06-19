#ifndef PLAYERAUDIO_H
#define PLAYERAUDIO_H

#include <QMediaPlayer>
#include <qmediaplaylist.h>

class PlayerAudio : public QObject
{
    Q_OBJECT
public:
    PlayerAudio(QObject *_parent = 0);
    virtual ~PlayerAudio();

    void                addUrl                      (const QString &_url);
    void                clearPlaylist               ();
    void                play                        ();
    void                pause                       ();
    /**
      * clamp 0<_pos<1
      * TODO if not fully load ?
      */
    void                setPositionRelative         (float _pos);
    /**
      * clamp 0<_ms<duration
      */
    void                setPosition                 (qint64 _ms);

    /**
      * ms
      */
    qint64              position                  () const;
    qint64              duration                 () const;

    /**
      * 0 not loaded
      * 1 fully loaded
      */
    //TODO
    float               getBufferRatio              () const;

    /**
      * clamp 0<v<100
      */
    void                setVolume                   (int _volume);

    void                setPlaylistPostion          (int _pos);

    int                 getPlaylistPosition         ();

public slots:
    void                next                        ();
    void                previous                    ();
signals:
    void                endOfTrack                  ();
    void                endOfPlaylist               ();
    //void                positionChanged             (qint64);
    void                positionChangedRel          (QVariant);
private:
    QMediaPlayer*       m_pPlayer;
    QMediaPlaylist*     m_pPlaylist;
private slots:
    void                stateChanged                (QMediaPlayer::State);
    void                playlistChanged             (int);
    void                computePosition             (qint64 _abs);
};

#endif // PLAYERAUDIO_H
