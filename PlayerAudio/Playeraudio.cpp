#include "Playeraudio.h"
#include <QDebug>

PlayerAudio::PlayerAudio(QObject *_parent)
    :   QObject         (_parent)
{
    m_pPlayer = new QMediaPlayer(this);
    m_pPlaylist = new QMediaPlaylist(this);

    m_pPlayer->setPlaylist(m_pPlaylist);

    bool ret = connect(m_pPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(stateChanged(QMediaPlayer::State)));
    Q_ASSERT(ret);
    connect(m_pPlayer, SIGNAL(positionChanged(qint64)), this, SIGNAL(positionChanged(qint64)));
    connect(m_pPlaylist, SIGNAL(currentIndexChanged(int)), this, SLOT(playlistChanged(int)));
}


PlayerAudio::~PlayerAudio()
{

}


void PlayerAudio::addUrl(const QString &_url)
{
    m_pPlaylist->addMedia(QMediaContent(QUrl(_url)));
}

void PlayerAudio::clearPlaylist()
{
    m_pPlaylist->clear();
}

void PlayerAudio::play()
{
    m_pPlayer->play();
}

void PlayerAudio::pause()
{
    m_pPlayer->pause();
}


void PlayerAudio::setPositionRelative(float _pos)
{
    if( _pos < 0.0 ) _pos = 0.0;
    if( _pos > 1.0 ) _pos = 1.0;

    qint64 absPos = (qint64)(m_pPlayer->duration() * _pos);
    m_pPlayer->setPosition(absPos);
}


void PlayerAudio::setPosition(qint64 _ms)
{
    m_pPlayer->setPosition(_ms);
}


qint64 PlayerAudio::position() const
{
    return m_pPlayer->position();
}


qint64 PlayerAudio::duration() const
{
    return m_pPlayer->duration();
}


void PlayerAudio::setVolume(int _volume)
{
    if(_volume < 0 ) _volume = 0;
    if(_volume > 100) _volume = 100;

    m_pPlayer->setVolume(_volume);
}

float PlayerAudio::getBufferRatio() const
{
    return -1;
//    const QIODevice *device = m_pPlayer->mediaStream();
//    Q_ASSERT(device);

//    qint64 read = device->bytesAvailable();
//    qint64 total = device->size();
//    return (float)total / (float)read;

}

int PlayerAudio::getPlaylistPosition()
{
    return m_pPlaylist->currentIndex();
}

void PlayerAudio::setPlaylistPostion(int _pos)
{
    m_pPlaylist->setCurrentIndex(_pos);
}

void PlayerAudio::next()
{
    m_pPlaylist->next();
}

void PlayerAudio::previous()
{
    m_pPlaylist->previous();

}
void PlayerAudio::stateChanged(QMediaPlayer::State _status)
{
    if( _status == QMediaPlayer::StoppedState) {
        emit endOfTrack();        
        if( m_pPlaylist->currentIndex() == -1) {
            emit endOfPlaylist();
        }
    }
}

void PlayerAudio::playlistChanged(int)
{
    if( m_pPlaylist->currentIndex() >= m_pPlaylist->mediaCount() - 1) {
        emit endOfTrack();
    }
}
