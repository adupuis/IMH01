#ifndef PLAYERAUDIO_H
#define PLAYERAUDIO_H

#include <QMediaPlayer>
#include <qmediaplaylist.h>

class PlayerAudio : public QObject
{
    Q_OBJECT
public:
    PlayerAudio();
    virtual ~PlayerAudio();

    void                addUrl                      (const QString &_url);
    void                clearPlaylist               ();
    void                play                        ();
    void                pause                       ();
    /**
      * clamp 0<_pos<1
      * TODO if not fully load ?
      */
    void                setPositionRelative         (qint64 _pos);
    /**
      * clamp 0<_ms<duration
      */
    void                setPosition                 (qint64 _ms);

    /**
      * ms
      */
    qint64              getDuration                 () const;

    /**
      * 0 not loaded
      * 1 fully loaded
      */
    int                 getBufferRatio              () const;

    /**
      * clamp 0<v<100
      */
    void                setVolume                   (int _volume);

    int                 getPlaylistPosition         ();

public slots:
    void                next                        ();
    void                previous                    ();
signals:
    void                endOfTrack                  ();
    void                endOfPlaylist               ();
private:
    QMediaPlayer*       m_pPlayer;
    QMediaPlaylist*     m_pPlaylist;
};

#endif // PLAYERAUDIO_H
