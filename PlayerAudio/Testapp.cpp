#include "Testapp.h"

TestApp::TestApp()
{
    m_player.setVolume(100);
    connect(&m_player, SIGNAL(endOfTrack()), this, SLOT(slotTrack()));
    connect(&m_player, SIGNAL(endOfPlaylist()), this, SLOT(slotPlaylist()));
    connect(&m_player, SIGNAL(positionChanged(qint64)), this, SLOT(slotPos(qint64)));

}

TestApp::~TestApp()
{

}



