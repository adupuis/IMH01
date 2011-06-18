#ifndef OAUTH_H
#define OAUTH_H

#include <Browser.h>

class Oauth : public QObject
{
    Q_OBJECT

public:

    Oauth();
    ~Oauth();

//    void setLogin( QString& _strLogin );
//    void setPassword( QString& _strLogin );

    QString getAccessToken();


private slots:

    void slotAccessTokenRetrieved( QString& _strAccessToken );


private:

    Browser*    m_browser;
    QString     m_strAccessToken;

};

#endif // OAUTH_H
