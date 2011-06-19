#include "Oauth.h"
#include <QDebug>

#include <Browser.h>

Oauth::Oauth()
 : m_strLogin       ( "" )
 , m_strPassword    ( "" )
 , m_strAccessToken ( "" )
{
    qDebug() << "Constructing OauthTest...";

    m_browser = new Browser;

    connect( m_browser, SIGNAL( sigAccessTokenRetrieved( QString& ) ),
             this,      SLOT(   slotAccessTokenRetrieved( QString& ) ) );
}

Oauth::~Oauth()
{
    delete m_browser;
}

void Oauth::setLogin( QString& _strLogin )
{
    m_strLogin = _strLogin;
}

void Oauth::setPassword( QString& _strPassword )
{
    m_strPassword = _strPassword;
}

QString Oauth::getAccessToken()
{
    return m_strAccessToken;
}

void Oauth::start()
{
    m_browser->setLogin( m_strLogin );
    m_browser->setPassword( m_strPassword );
    m_browser->processSoundCloudOAuthPage();
}

void Oauth::slotAccessTokenRetrieved( QString& _strAccessToken )
{
    qDebug() << "access token retrieved:" << _strAccessToken;
    emit sigAccessTokenAvailable( m_strAccessToken );
}
