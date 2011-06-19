#include <Browser.h>

Browser::Browser()
    : m_strLogin                ( "" )
    , m_strPassword             ( "" )
    , m_bLoadFinished           ( false )
    , m_iNbRedirections         ( 0 )
    , m_iNbMaxRedirections      ( 5 )
    , m_strUrl                  ( "" )

{
    qDebug() << "Constructing Browser object...";

    m_networkAccessManager = new QNetworkAccessManager;

    m_webView = new QWebView;

    QWebPage* webPage = m_webView->page();
    webPage->setLinkDelegationPolicy( QWebPage::DelegateAllLinks );

    m_networkAccessManager = m_webView->page()->networkAccessManager();

    connect( m_webView, SIGNAL( loadStarted() ),
             this,      SLOT(   slotHandleLoadStarted() ),
            Qt::UniqueConnection );

    connect( m_webView, SIGNAL( loadProgress( int ) ),
             this,      SLOT(   slotHandleLoadProgress( int ) ),
            Qt::UniqueConnection );

    connect( m_webView, SIGNAL( loadFinished( bool ) ),
             this,      SLOT(   slotHandleLoadFinished( bool ) ),
            Qt::UniqueConnection );

    connect( m_webView, SIGNAL( linkClicked( const QUrl& ) ),
             this,      SLOT(   slotHandleLinkClicked( const QUrl& ) ),
            Qt::UniqueConnection );

    connect( m_webView, SIGNAL( urlChanged( const QUrl& ) ),
             this,      SLOT(   slotHandleUrlChanged( const QUrl& ) ),
            Qt::UniqueConnection );

    connect( m_networkAccessManager, SIGNAL( sslErrors( QNetworkReply* , QList<QSslError> ) ),
             this,                   SLOT(   slotHandleSslErrors( QNetworkReply* , QList<QSslError> ) ),
            Qt::UniqueConnection );


    m_loadTimer.setSingleShot( true );

    connect( &m_loadTimer,  SIGNAL( timeout() ),
             this,          SLOT(   slotTimerTimeout() ),
            Qt::UniqueConnection );
}

Browser::~Browser()
{
    if( m_webView )
    {
        delete m_webView;
    }
//        if( m_networkAccessManager )
//        {
//            delete m_networkAccessManager;
//        }
}

QString Browser::getLogin()
{
    return m_strLogin;
}

void Browser::setLogin( const QString& _strLogin )
{
    m_strLogin = _strLogin;
}

void Browser::setPassword( const QString& _strPassword )
{
    m_strPassword = _strPassword;
}

QString Browser::getAccessToken()
{
    return m_strAccessToken;
}

void Browser::processSoundCloudOAuthPage()
{
    qDebug() << "Processing SoundCloud authorization page...";

    QString strOAuthUrl = "https://soundcloud.com/connect?client_id=ef7c3301f5a463034354f0bfa1ee0236&redirect_uri=http://soundcloud.com/dashboard&response_type=token&display=popup";

    qDebug() << "Loading oauth url into webview:" << strOAuthUrl;
    startRequest( strOAuthUrl );
}

QNetworkRequest Browser::setSSLConfig()
{
    QNetworkRequest networkRequest;

    QSslConfiguration sslConfig = networkRequest.sslConfiguration();
    sslConfig.setPeerVerifyMode( QSslSocket::VerifyNone );
    sslConfig.setProtocol( QSsl::TlsV1 );

    QNetworkRequest request( networkRequest );
    request.setSslConfiguration( sslConfig );

    return request;
}

void Browser::startRequest( QString& _strUrl )
{
    qDebug() << "Creating request and connecting to url...";

    m_strUrl = _strUrl;

    QNetworkRequest networkRequest = setSSLConfig();
    networkRequest.setUrl( QUrl( _strUrl ) );

    m_networkReply = m_networkAccessManager->get( networkRequest );

    connect( m_networkReply, SIGNAL( finished() ),
             this,           SLOT(   slotHandleNetworkData() ),
            Qt::UniqueConnection );
}

void Browser::loadUrl( QString& _strUrl )
{
    QNetworkRequest networkRequest = setSSLConfig();
    networkRequest.setUrl( QUrl( _strUrl ) );
    m_webView->load( networkRequest );
    m_webView->show();
}

void Browser::slotHandleNetworkData()
{
    qDebug() << "Handle network data.";

    if( !m_networkReply->error() )
    {
        qDebug() << "no error in reply.";
        int iHttpStatusCodeAttribute = m_networkReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
        qDebug() << "HttpStatusCodeAttribute:" << iHttpStatusCodeAttribute;

        QString strHeader = m_strUrl;
        // 302 HTTP status code means a redirection was found
        if( iHttpStatusCodeAttribute == 302 )
        {
            m_iNbRedirections++;
            if( m_iNbRedirections <= m_iNbMaxRedirections )
            {
                qDebug() << "Applying redirection";
                strHeader = QString( m_networkReply->header( QNetworkRequest::LocationHeader ).toString() );
                qDebug() << "strHeader:" << strHeader;

                if( strHeader.isEmpty() )
                {
                    loadUrl( m_strUrl );
                }
                else
                {
                    m_networkReply->abort();
                    m_networkReply->deleteLater();
                    startRequest( strHeader );
                }
            }
            else
            {
                qDebug() << "404 Error! URL not found.";
            }
        }
        else
        {
            loadUrl( strHeader );
        }
    }
    else
    {
        qDebug() << "error:" << m_networkReply->errorString();
    }
    m_networkReply->abort();
    m_networkReply->deleteLater();
}

void Browser::slotHandleLoadStarted()
{
    qDebug() << "Load started.";

    if( m_loadTimer.isActive() )
    {
        qDebug() << "Timer already active. Stopping old timer.";
        m_loadTimer.stop();
    }
    qDebug() << "Starting new timer.";
    m_loadTimer.start( 45000 );
}

void Browser::slotTimerTimeout()
{
    qDebug() << "TIMEOUT!";
    if( !m_bLoadFinished )
    {
        m_webView->stop();
        QString strErrorMessage = "Action timed out!";
        emit sigOAuthActionFailed( 0, strErrorMessage );
    }
}

void Browser::slotHandleLoadProgress( int _iProgress )
{
    qDebug() << "Page loading... " << _iProgress;
}

void Browser::slotHandleLoadFinished( bool _ok )
{
    m_bLoadFinished = true;

    QUrl currentUrl = m_webView->url();
    QString strCurrentUrl = currentUrl.toString();
    qDebug() << "Current URL:" << strCurrentUrl;




    qDebug() << "Will parse page just loaded...";
    if( !_ok )
    {
        qDebug() << "Error while loading page! (this might be normal)";
    }
    else
    {
        QWebFrame* mainFrame = m_webView->page()->mainFrame();

        qDebug() << "Page loaded.";

        // Parsing oauth or login page for processing login/authorization action
        qDebug() << "Start parsing inputs.";
        QWebElementCollection inputs = mainFrame->documentElement().findAll( "input" );
        foreach( QWebElement input, inputs )
        {
            qDebug() << "id:" << input.attribute( "id" );
            if( input.attribute( "id" ) == "username" )
            {
                qDebug() << "id:" << input.attribute( "id" ) << "- value:" << input.attribute( "value" );
                input.setAttribute( "value", m_strLogin );
            }
            else if( input.attribute( "id" ) == "password" )
            {
                qDebug() << "id:" << input.attribute( "id" ) << "- value:" << input.attribute( "value" );
                input.setAttribute( "value", m_strPassword );
            }


//            else if( input.attribute( "type" ) == "submit" && input.attribute( "name" ) == "login" )
//            {
////                        OV_FLASHING_MSG( "id: %s - value: %s", input.attribute( "id" ).toUtf8().constData(),  input.attribute( "value" ).toUtf8().constData() );
//                input.evaluateJavaScript( "this.click()" );
//                if( m_bLoginAction )
//                {
//                    m_bJustLoggedIn = true;
//                    m_bLoginDone = true;
//                }
//                if( m_bOAuthAction )
//                {
//                    m_bOAuthDone = true;
//                }
////                        break;  // TODO : test this!!!
//            }
//            else if( input.attribute( "type" ) == "submit" && input.attribute( "name" ) == "grant_clicked" )
//            {
////                        OV_FLASHING_MSG( "id: %s - value: %s", input.attribute( "id" ).toUtf8().constData(),  input.attribute( "value" ).toUtf8().constData() );
//                input.evaluateJavaScript( "this.click()" );
//            }
        }
        qDebug() << "Done parsing inputs.";
    }













    m_bLoadFinished = false;
}

void Browser::slotHandleLinkClicked( const QUrl& _url )
{
    QString strUrl = _url.toString();

    qDebug() << "You clicked on a link pointing to this URL:";
    qDebug() << strUrl;

    startRequest( strUrl );
}

void Browser::slotHandleUrlChanged( const QUrl& _url )
{
    QString strUrl = _url.toString();
    qDebug() << "URL changed:" << strUrl;

    // Parsing URL of oauth request's result for finding authentication access_token
    if( strUrl.contains( "access_token=" ) )
    {
        QString strAccessToken;
        QStringList urlSplit = strUrl.split( "access_token=" );
        strAccessToken = urlSplit.at( 1 );
        if( strAccessToken.contains( "&expires_in" ) )
        {
            urlSplit = strAccessToken.split( "&expires_in" );
            strAccessToken = urlSplit.at( 0 );
        }
        qDebug() << "access_token" << strAccessToken;
        m_strAccessToken = strAccessToken;
        m_webView->close();
        emit sigAccessTokenRetrieved( strAccessToken );
    }
}

void Browser::slotHandleSslErrors( QNetworkReply* _networkReply , QList<QSslError> _sslErrors )
{
//    qDebug() << "Request contains SSL errors. Ignoring it." );
//        foreach( QSslError sslError, _sslErrors)
//        {
//            qDebug() << "SSL error: %s", sslError.errorString().toUtf8().constData() );
//        }
    _networkReply->ignoreSslErrors();
}
