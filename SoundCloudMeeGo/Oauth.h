#ifndef OAUTH_H
#define OAUTH_H

#include <Browser.h>

class Oauth : public QObject
{
    Q_OBJECT

public:

    Oauth();
    ~Oauth();

    void setLogin( QString& _strLogin );
    void setPassword( QString& _strPassword );

    QString getAccessToken();

    void start();


signals:

    void sigAccessTokenAvailable( QString& );


private slots:

    void slotAccessTokenRetrieved( const QString& _strAccessToken );


private:

    Browser*    m_browser;

    QString     m_strLogin;
    QString     m_strPassword;

    QString     m_strAccessToken;

};

#endif // OAUTH_H
