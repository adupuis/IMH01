#ifndef BROWSER_H
#define BROWSER_H

#include <QtWebKit>
#include <QtNetwork>


class Browser : public QObject
{
    Q_OBJECT

public:

    Browser();

    ~Browser();

//    QString             getEmail();

//    void                setEmail( const QString& _strEmail );

//    void                setPassword( const QString& _strPassword );

    QString             getAccessToken();

    void                processSoundCloudOAuthPage();

private:

    /// Modify SSL configuration of a request (for avoiding SSL handshake failure)
    QNetworkRequest     setSSLConfig();

    void                startRequest( QString& _strUrl );

    void                loadUrl( QString& _strUrl );


signals:

    void sigOAuthActionFailed( const int _iId, const QString& _strErrorMessage );

    void sigAccessTokenRetrieved( const QString& _strAccessToken );


private slots:

    void slotHandleNetworkData();

    void slotHandleLoadStarted();

    void slotTimerTimeout();

    void slotHandleLoadProgress( int _iProgress );

    void slotHandleLoadFinished( bool _ok );

    void slotHandleLinkClicked( const QUrl& _url );

    void slotHandleUrlChanged( const QUrl& _url );

    void slotHandleSslErrors( QNetworkReply* _networkReply , QList<QSslError> _sslErrors );


private:

    QWebView*               m_webView;

    QNetworkAccessManager*  m_networkAccessManager;

    QNetworkReply*          m_networkReply;

    QString                 m_strEmail;

    QString                 m_strPassword;

    QString                 m_strAccessToken;

    bool                    m_bLoadFinished;

    QTimer                  m_loadTimer;

    int                     m_iNbRedirections;

    int                     m_iNbMaxRedirections;

    QString                 m_strUrl;
};

#endif // BROWSER_H
