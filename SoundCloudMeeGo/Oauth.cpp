#include "Oauth.h"
#include <QDebug>

#include <Browser.h>

Oauth::Oauth()
 : m_strAccessToken ( "" )
{
    qDebug() << "Constructing OauthTest...";

    m_browser = new Browser;

    connect( m_browser, SIGNAL( sigAccessTokenRetrieved( QString& ) ),
             this,      SLOT(   slotAccessTokenRetrieved( QString& ) ) );

    m_browser->processSoundCloudOAuthPage();
}

Oauth::~Oauth()
{
    delete m_browser;
}

QString Oauth::getAccessToken()
{
    return m_strAccessToken;
}

void Oauth::slotAccessTokenRetrieved( QString& _strAccessToken )
{
    qDebug() << "access token retrieved:" << _strAccessToken;
    emit sigAccessTokenAvailable( m_strAccessToken );
}
