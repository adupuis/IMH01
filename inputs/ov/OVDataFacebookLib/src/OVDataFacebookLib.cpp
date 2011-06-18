/*!
    FacebookLib.cpp
    Quentin Désert
    Orange Vallée - 2010
*/

#include <OVDataFacebookLib.h>
#include <OVDataFacebookDebug.h>

#include <OVDebugAllocation.h>

// openssl & openssl-devel must be installed for this lib to work!

#define OVFACEBOOKCONFIGINI "OVFacebookConfig.ini"

namespace OVData
{

    FacebookLib::FacebookLib( QObject *parent ) :
        QObject( parent )
    //  , m_mutex( QMutex::Recursive )
    {
        OVFB_MSG( "Creating Facebook object..." );

        QString strConfigToParse = OVFACEBOOKCONFIGINI;
        m_parser = OV_NEW(Domaine_App) OVSmartParser( strConfigToParse );

        m_iNbPagesToSave = 5;

        m_iNbRedirections = 0;
        m_iNbMaxRedirections = 5;

        m_iNbDownloadAttempts = 0;

        m_strHandleDataActionTag = "default";

        m_strCurrentUserId = "";
        m_strCurrentAccessToken = "";

        m_strClientId = m_parser->getConfigValue( "client_id" ).toString();
        m_strClientSecret = m_parser->getConfigValue( "client_secret" ).toString();
        m_strRedirectUri = m_parser->getConfigValue( "redirect_uri" ).toString();
        m_strPermissionsScope = m_parser->getConfigValue( "permissions_scope" ).toString();
        m_strOAuthBaseUrl = m_parser->getConfigValue( "oauth_url" ).toString();
        m_strGetAccessTokenBaseUrl = m_parser->getConfigValue( "get_access_token_url" ).toString();

        m_strFriendsListUrl = m_parser->getConfigValue( "friends_list_url" ).toString();
        m_strSearchUrl = m_parser->getConfigValue( "search_url" ).toString();
        m_strUserFeedPostListUrl = m_parser->getConfigValue( "user_feed_post_list_url" ).toString();
        m_strUserIdUrl = m_parser->getConfigValue( "user_id_url" ).toString();
        m_strUserNameUrl = m_parser->getConfigValue( "user_name_url" ).toString();
        m_strUserPictureUrl = m_parser->getConfigValue( "user_picture_url" ).toString();
        m_strUserLikesUrl = m_parser->getConfigValue( "user_likes_url" ).toString();

        m_strFanPageUrl = m_parser->getConfigValue( "fan_page_url" ).toString();
        m_strFanPagePostListUrl = m_parser->getConfigValue( "fan_page_post_list_url" ).toString();

        m_strPostMessageUrl = m_parser->getConfigValue( "post_message_url" ).toString();

        m_networkAccessManager = OV_NEW(Domaine_App) QNetworkAccessManager( this );

    //    connect( m_networkAccessManager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( handleNetworkDataSlot( QNetworkReply* ) ), Qt::DirectConnection );
    }

    FacebookLib::~FacebookLib()
    {
        OV_DELETE( m_networkAccessManager );
    //    delete m_sentReply;
        OV_DELETE( m_parser );
    }

    QNetworkRequest FacebookLib::setSSLConfig()
    {
        QNetworkRequest networkRequest;

        QSslConfiguration sslConfig = networkRequest.sslConfiguration();
        sslConfig.setPeerVerifyMode( QSslSocket::VerifyNone );
        sslConfig.setProtocol( QSsl::TlsV1 );

        QNetworkRequest request( networkRequest );
        request.setSslConfiguration( sslConfig );

        return request;
    }

    void FacebookLib::startRequest( QString& _strUrl )
    {
    //    QMutexLocker locker( &m_mutex );

    //    OVFB_MSG( "Creating request and connecting to Facebook API..." );

        m_strUrl = _strUrl;

        QNetworkRequest networkRequest = setSSLConfig();
        networkRequest.setUrl( QUrl( _strUrl ) );

    //    m_downloadTimer.setSingleShot( true );

        m_networkReply = m_networkAccessManager->get( networkRequest );

        OV_CONNECT( m_networkReply, SIGNAL( finished() ),
                    this,           SLOT(   slotHandleNetworkData() ),
                   Qt::UniqueConnection );

    //    m_downloadTimer.start( 15000 );

    //    connect( &m_downloadTimer, SIGNAL( timeout() ), &q, SLOT( quit() ) );
    //    connect( m_networkReply, SIGNAL( downloadProgress( qint64, qint64 ) ), this, SLOT( checkNetworkReplyProgress( qint64, qint64 ) ), Qt::DirectConnection );
    //    connect( m_networkReply, SIGNAL( finished() ), this, SLOT( slotHandleNetworkData() ), Qt::DirectConnection );
    //    connect( m_networkReply, SIGNAL( error( QNetworkReply::NetworkError ) ), this, SLOT( slotHandleNetworkError( QNetworkReply::NetworkError ) ) );
    }

    QString FacebookLib::getClientId()
    {
        return m_strClientId;
    }

    QString FacebookLib::getClientSecret()
    {
        return m_strClientSecret;
    }

    QString FacebookLib::getRedirectUri()
    {
        return m_strRedirectUri;
    }

    QString FacebookLib::getPermissionsScope()
    {
        return m_strPermissionsScope;
    }

    QString FacebookLib::getOAuthUrl()
    {
        return m_strOAuthUrl;
    }

    QString FacebookLib::getCurrentUserId()
    {
        return m_strCurrentUserId;
    }

    QString FacebookLib::getGetAccessTokenUrl()
    {
        return m_strGetAccessTokenUrl;
    }

    QString FacebookLib::getCurrentAccessToken()
    {
        return m_strCurrentAccessToken;
    }

    void FacebookLib::setCurrentAccessToken( const QString& _strAccessToken )
    {
        m_strCurrentAccessToken = _strAccessToken;
    }

    QMap< int, QMap<QString, QString> > FacebookLib::getFriendsList()
    {
        return m_friendsList;
    }

    QStringList FacebookLib::getFanPagesIdsFound()
    {
        return m_fanPagesIdsFound;
    }

    QString FacebookLib::getUserIdFound()
    {
        return m_strUserIdFound;
    }

    QString FacebookLib::getUserNameFound()
    {
        return m_strUserNameFound;
    }

    QImage FacebookLib::getUserPictureFound()
    {
        return m_userPictureFound;
    }

    QImage FacebookLib::getFriendPictureFound()
    {
        return m_friendPictureFound;
    }

    QStringList FacebookLib::getUserLikesFound()
    {
        return m_userLikesFound;
    }

    QMap< int, QMap<QString, QString> > FacebookLib::getUserFeedPostListFound()
    {
        return m_userFeedPostListFound;
    }

    QString FacebookLib::getUserFeedPosterNameFound()
    {
        return m_strUserFeedPosterNameFound;
    }

    QImage FacebookLib::getUserFeedPosterPictureFound()
    {
        return m_UserFeedPosterPictureFound;
    }

    QMap<QString, QString> FacebookLib::getFanPageInformationFound()
    {
        return m_fanPageInformation;
    }

    QImage FacebookLib::getFanPagePictureFound()
    {
        return m_fanPagePicture;
    }

    QMap< int, QMap<QString, QString> > FacebookLib::getFanPagePostListFound()
    {
        return m_FanPagePostListFound;
    }

    QString FacebookLib::getFanPagePosterNameFound()
    {
        return m_strFanPagePosterNameFound;
    }

    QImage FacebookLib::getFanPagePosterPictureFound()
    {
        return m_fanPagePosterPictureFound;
    }

    QString FacebookLib::getPostId()
    {
        return m_strPostId;
    }

    void FacebookLib::searchFanPage( QString& _strContent )
    {
        m_strContent = _strContent;

        QString strSearchUrl = m_strSearchUrl;
        strSearchUrl.replace( "<content>", m_strContent );
        strSearchUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Searching fan page for content %s : %s", m_strContent.toUtf8().constData(), strSearchUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "search";
        startRequest( strSearchUrl );
    }

    void FacebookLib::retrieveUserId()
    {
        QString strUserIdUrl = m_strUserIdUrl;
        strUserIdUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving user ID for user with access token %s : %s", m_strCurrentAccessToken.toUtf8().constData(), strUserIdUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "user_id";
        startRequest( strUserIdUrl );
    }

    void FacebookLib::retrieveUserName( const QString& _strUserId )
    {
        QString strUserNameUrl = m_strUserNameUrl;
        strUserNameUrl.replace( "<user_id>", _strUserId );
        strUserNameUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving user name for user %s : %s", _strUserId.toUtf8().constData(), strUserNameUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "user_name";
        startRequest( strUserNameUrl );
    }

    void FacebookLib::retrieveUserPicture( const QString& _strUserId )
    {
        QString strUserPictureUrl = m_strUserPictureUrl;
        strUserPictureUrl.replace( "<user_id>", _strUserId );
        strUserPictureUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving picture for user %s : %s", _strUserId.toUtf8().constData(), strUserPictureUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "user_picture";
        startRequest( strUserPictureUrl );
    }

    void FacebookLib::retrieveFriendPicture( const QString& _strFriendId )
    {
        QString strFriendPictureUrl = m_strUserPictureUrl;
        strFriendPictureUrl.replace( "<user_id>", _strFriendId );
        strFriendPictureUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving picture for friend %s : %s", _strFriendId.toUtf8().constData(), strFriendPictureUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "friend_picture";
        startRequest( strFriendPictureUrl );
    }

    void FacebookLib::retrieveFriendsList( QString& _strUserId )
    {
        QString strFriendsListUrl = m_strFriendsListUrl;
        strFriendsListUrl.replace( "<user_id>", _strUserId );
        strFriendsListUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving friends list for user %s : %s", _strUserId.toUtf8().constData(), strFriendsListUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "friends";
        startRequest( strFriendsListUrl );
    }

    void FacebookLib::retrieveUserLikes( const QString& _strUserId )
    {
        QString strUserLikesUrl = m_strUserLikesUrl;
        strUserLikesUrl.replace( "<user_id>", _strUserId );
        strUserLikesUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving likes for user %s : %s", _strUserId.toUtf8().constData(), strUserLikesUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "user_likes";
        startRequest( strUserLikesUrl );
    }

    void FacebookLib::retrieveUserFeedPostList( QString& _strUserId )
    {
        QString strUserFeedPostListUrl = m_strUserFeedPostListUrl;
        strUserFeedPostListUrl.replace( "<user_id>", _strUserId );
        strUserFeedPostListUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving user feed post list for user %s : %s", _strUserId.toUtf8().constData(), strUserFeedPostListUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "user_feed_post_list";
        startRequest( strUserFeedPostListUrl );
    }

    void FacebookLib::retrieveUserFeedPosterName( const QString& _strUserFeedPosterId )
    {
        QString strUserFeedPostNameUrl = m_strUserNameUrl;
        strUserFeedPostNameUrl.replace( "<user_id>", _strUserFeedPosterId );
        strUserFeedPostNameUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving name for user feed poster %s : %s", _strUserFeedPosterId.toUtf8().constData(), strUserFeedPostNameUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "user_feed_poster_name";
        startRequest( strUserFeedPostNameUrl );
    }

    void FacebookLib::retrieveUserFeedPosterPicture( const QString& _strUserFeedPosterId )
    {
        QString strUserFeedPosterPictureUrl = m_strUserPictureUrl;
        strUserFeedPosterPictureUrl.replace( "<user_id>", _strUserFeedPosterId );
        strUserFeedPosterPictureUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving picture for user feed poster %s : %s", _strUserFeedPosterId.toUtf8().constData(), strUserFeedPosterPictureUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "user_feed_poster_picture";
        startRequest( strUserFeedPosterPictureUrl );
    }

    void FacebookLib::retrieveFanPageInformation( QString& _strFanPageId )
    {
        QString strFanPageUrl = m_strFanPageUrl;
        strFanPageUrl.replace( "<page_id>", _strFanPageId );
        strFanPageUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving information for fan page %s : %s", _strFanPageId.toUtf8().constData(), strFanPageUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "fan_page_information";
        startRequest( strFanPageUrl );
    }

    //void FacebookLib::retrieveFanPagePicture( QString& _strFanPageId )
    //{
    //    OVFB_MSG( "Getting picture for fan page: %s", _strFanPageId.toUtf8().constData() );
    //    QString strFanPagePictureUrl = m_strFanPagePictureUrl;
    //    strFanPagePictureUrl.replace( "<page_id>", _strFanPageId );

    //    OVFB_MSG( "strFanPagePictureUrl: %s", strFanPagePictureUrl.toUtf8().constData() );

    //    m_strHandleDataActionTag = "fan_page_picture";

    //    QNetworkRequest networkRequest = setSSLConfig();
    //    networkRequest.setUrl( QUrl( strFanPagePictureUrl ) );
    //    m_networkAccessManager->get( networkRequest );
    //}

    void FacebookLib::retrieveFanPagePicture( QString& _strFanPageId )
    {
        QString strFanPageUrl = m_strFanPageUrl;
        strFanPageUrl.replace( "<page_id>", _strFanPageId );
        strFanPageUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving picture for fan page %s : %s", _strFanPageId.toUtf8().constData(), strFanPageUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "fan_page_picture";
        startRequest( strFanPageUrl );
    }

    void FacebookLib::retrieveFanPagePostList( QString& _strFanPageId )
    {
        QString strFanPagePostListUrl = m_strFanPagePostListUrl;
        strFanPagePostListUrl.replace( "<page_id>", _strFanPageId );
        strFanPagePostListUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving post list for fan page %s : %s", _strFanPageId.toUtf8().constData(), strFanPagePostListUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "fan_page_post_list";
        startRequest( strFanPagePostListUrl );
    }

    void FacebookLib::retrieveFanPagePosterName( const QString& _strFanPagePosterId )
    {
        QString strFanPagePostNameUrl = m_strUserNameUrl;
        strFanPagePostNameUrl.replace( "<user_id>", _strFanPagePosterId );
        strFanPagePostNameUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving name for fan page poster %s : %s", _strFanPagePosterId.toUtf8().constData(), strFanPagePostNameUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "fan_page_poster_name";
        startRequest( strFanPagePostNameUrl );
    }

    void FacebookLib::retrieveFanPagePosterPicture( const QString& _strFanPagePosterId )
    {
        QString strFanPagePosterPictureUrl = m_strUserPictureUrl;
        strFanPagePosterPictureUrl.replace( "<user_id>", _strFanPagePosterId );
        strFanPagePosterPictureUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Retrieving picture for fan page poster %s : %s", _strFanPagePosterId.toUtf8().constData(), strFanPagePosterPictureUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "fan_page_poster_picture";
        startRequest( strFanPagePosterPictureUrl );
    }

    void FacebookLib::postMessage( const QString& _strUserId, QString& _strLink, QString& _strMessage )
    {
        QString strPostMessageUrl = m_strPostMessageUrl;
        strPostMessageUrl.replace( "<user_id>", _strUserId );
        if( _strLink.contains( "http://" ) )    // TODO: the check should be better...
        {
            strPostMessageUrl.replace( "<link>", _strLink );
        }
        else
        {
            strPostMessageUrl.replace( "<message>&link=<link>", _strLink + ": <message>" );
        }
        _strMessage = escapeSpecialChars( _strMessage );
    //    OVFB_MSG( "_strMessage escaped: %s", _strMessage.toUtf8().constData() );
        strPostMessageUrl.replace( "<message>", _strMessage );
        strPostMessageUrl.replace( "<access_token>", m_strCurrentAccessToken );

        OVFB_MSG( "Posting message on wall of user %s : %s", _strUserId.toUtf8().constData(), strPostMessageUrl.toUtf8().constData() );

        m_strHandleDataActionTag = "post_message";
        startRequest( strPostMessageUrl );
    }

    QString FacebookLib::convertDate( QString& _strApiDate )

    {
        _strApiDate.remove( 0, 5 );

        QString strMonth = _strApiDate.left( 2 );
        strMonth = m_parser->getConfigValue( strMonth ).toString();

        _strApiDate.remove( 0, 3 );

        QString strDay = _strApiDate.left( 2 );

        _strApiDate.remove( 0, 3 );

        QString strTime = _strApiDate.left( 5 );

        QString strNiceDate = strMonth;
        strNiceDate.append( " " )
                   .append( strDay )
                   .append( " at " )
                   .append( strTime );

        return strNiceDate;
    }

    QString FacebookLib::replaceSpecialChars( QString& _strString )
    {
        if( _strString.contains( "\\u" ) )
        {
            QStringList parts = _strString.split( "\\u" );
            QString strCode = parts.at( 1 ).left( 4 );
            bool ok;
            int hexaCode = QString( "0x" + strCode ).toInt( &ok, 16 );
            QChar c( hexaCode );
            _strString.replace( QString( "\\u" ) + strCode, c );

            _strString = replaceSpecialChars( _strString );
        }
        if( _strString.contains( "\\n" ) )
        {
    //        _strString.replace( "\\n", QChar((int)'\n' ) );
    //        _strString.replace( "\\n", "" ) );
            _strString.remove( "\\n" );
        }
        if( _strString.contains( "\'" ) )
        {
            _strString.replace( "\'", "`" );
        }
        if( _strString.contains( "\\/" ) )
        {
            _strString.replace( "\\/", "/" );
        }
        if( _strString.contains( "\\\"" ) )
        {
            _strString.replace( "\\\"", "\"" );
        }
        _strString.remove( QRegExp( "[\\n\\t\\r]" ) );
        _strString = _strString.simplified();
        return _strString;
    }

    QString FacebookLib::escapeSpecialChars( QString& _strString )
    {
        QString strEncoded( QUrl::toPercentEncoding( _strString ) );
        return strEncoded;
    }

    QString FacebookLib::removePercentEncoding( QString& _strString )
    {
        QString strDecoded( QUrl::fromPercentEncoding( _strString.toUtf8() ) );
        return strDecoded;
    }

    //void FacebookLib::checkNetworkReplyProgress( qint64 _bytesReceived, qint64 _bytesTotal )
    //{
    //    OVFB_FLASHING_MSG( "Total to download: %lld", _bytesTotal );
    //    OVFB_FLASHING_MSG( "Download progress: %lld", _bytesReceived );

    //    if( _bytesTotal < 0 )
    //    {
    //        OVFB_FLASHING_MSG( "Cannot find download size, retrying..." );
    //        m_iNbDownloadAttempts++;
    //        if( m_iNbDownloadAttempts <= 2 )
    //        {
    //            disconnect( m_networkReply, SIGNAL( finished() ), this, SLOT( slotHandleNetworkData() ) );
    ////            m_networkReply->disconnect( SLOT( handleNetworkDataSlot() ) );
    ////            m_networkReply->close();
    //            m_networkReply->abort();
    //            m_networkReply->deleteLater();
    //            startRequest( m_strUrl );
    //        }
    //        else
    //        {
    //            m_iNbDownloadAttempts = 0;
    //            emit sigRequestFailed();
    //        }
    //    }
    //}

    //void FacebookLib::slotHandleNetworkError( QNetworkReply::NetworkError _error )
    //{
    //    OVFB_WARNING( "Network Error: %s", m_networkReply->errorString().toUtf8().constData() );
    //}

    //void FacebookLib::slotHandleNetworkData( QNetworkReply* _networkReply )
    void FacebookLib::slotHandleNetworkData()
    {
    //    OVFB_MSG( "Handling Network Data. Action: %s", m_strHandleDataActionTag.toUtf8().constData() );

    //    if( _networkReply == m_sentReply )
    //    {
    //        OVFB_MSG( "Reply OK!" );

            if( m_strHandleDataActionTag == "search" )
            {
                m_fanPagesIdsFound.clear();
                if( !m_networkReply->error() )
                {

                    QByteArray response( m_networkReply->readAll() );
                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

    //                OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != "" && strJsonResponse != "{\"data\":[]}" )
                    {
                        if( strJsonResponse.contains( "[{" ) )
                        {
                            QStringList split1 = strJsonResponse.split( "[{" );
                            if( split1.at( 1 ).contains( "}]" ) )
                            {
                                QStringList split2 = split1.at( 1 ).split( "}]" );
                                QStringList elements;
                                if( split2.at( 0 ).contains( "},{" ) )
                                {
                                    // Multiple entries found
                                    elements = split2.at( 0 ).split( "},{" );
                                }
                                else
                                {
                                    // Only one entry found
                                    elements = split2;
                                }

                                QMap<QString, QString> tmpMap;
                                QMap< int, QMap<QString, QString> > exactMatchPagesMap;
                                QMap< int, QMap<QString, QString> > pagesMap;

                                int i = 0;
                                int j = 0;
                                foreach( QString element, elements )
                                {
                                    QString strTmp = element.remove( 0, 8 );
                                    if( strTmp.contains( ",\"" ) )
                                    {
                                        QStringList tmpList = strTmp.split( ",\"" );

                                        QString strTmpLikes = tmpList.at( 0 );

                                        QString strTmpCategory = tmpList.at( 1 );
                                        strTmpCategory = strTmpCategory.remove( 0, 11 );
                                        strTmpCategory.chop( 1 );

                                        if( strTmp.contains( "\",\"" ) )
                                        {
                                            tmpList = strTmp.split( "\",\"" );

                                            strTmp = tmpList.at( 1 );
                                            QString strTmpId = strTmp.remove( 0, 5 );

                                            strTmp = tmpList.at( 2 );
                                            QString strTmpName = strTmp.remove( 0, 7 );
                                            strTmpName.chop( 1 );

                                            if( strTmpCategory == "Film"
                                             || strTmpCategory == "Movie"
                                             || strTmpCategory == "Tv"
                                             || strTmpCategory == "Television"
                                             || strTmpCategory == "Tv show"
                                            )
                                            {
                                                tmpMap["likes"] = strTmpLikes;
                                                tmpMap["category"] = strTmpCategory;
                                                tmpMap["id"] = strTmpId;
                                                tmpMap["name"] = strTmpName;

                                                if( strTmpName.compare( m_strContent, Qt::CaseInsensitive ) == 0 )
                                                {
                                                    exactMatchPagesMap[j] = tmpMap;
                                                    j++;
                                                }
                                                pagesMap[i] = tmpMap;
                                                i++;
                                            }
                                        }
                                        else
                                        {
                                            OVFB_MSG( "No separator found for fan page!" );
                                        }
                                    }
                                    else
                                    {
                                        OVFB_MSG( "No separator found for fan page!" );
                                    }
                                }
                                int iNbFanPagesMatching = exactMatchPagesMap.size();

                                bool bExactMatchEmpty = false;
                                if( iNbFanPagesMatching == 0 )
                                {
                                    bExactMatchEmpty = true;
                                    iNbFanPagesMatching = pagesMap.size();
                                }

                                // Save the 'm_iNbPagesToSave' first pages of the list of fan pages of category 'Film', 'Movie', 'Tv' or 'Television'
            //                    int iNbPagesToSave = 5;
                                QMap<int, QString> fanPagesMap;
                                for( int p = 0; p < m_iNbPagesToSave; p++ )
                                {
                                    if( p < iNbFanPagesMatching )
                                    {
                                        if( !bExactMatchEmpty )
                                        {
                                            OVFB_MSG( "fanpage saved (exact match) | id: %s - name: %s, category: %s, likes: %s",
                                                     exactMatchPagesMap[p]["id"].toUtf8().constData(),
                                                     exactMatchPagesMap[p]["name"].toUtf8().constData(),
                                                     exactMatchPagesMap[p]["category"].toUtf8().constData(),
                                                     exactMatchPagesMap[p]["likes"].toUtf8().constData() );

                                            int iFanPageLikes = exactMatchPagesMap[p]["likes"].toInt();
                                            if( fanPagesMap.contains( iFanPageLikes ) )
                                            {
                                                while( fanPagesMap.contains( iFanPageLikes ) )
                                                {
                                                    OVFB_MSG("fanPagesMap already contains this key, decreasing..." );
                                                    iFanPageLikes -=1;
                                                }
                                            }
                                            fanPagesMap[iFanPageLikes] = exactMatchPagesMap[p]["id"];
                                        }
                                        else
                                        {
                                            OVFB_MSG( "fanpage saved (not exact match) | id: %s - name: %s, category: %s, likes: %s",
                                                     pagesMap[p]["id"].toUtf8().constData(),
                                                     pagesMap[p]["name"].toUtf8().constData(),
                                                     pagesMap[p]["category"].toUtf8().constData(),
                                                     pagesMap[p]["likes"].toUtf8().constData() );

                                            int iFanPageLikes = pagesMap[p]["likes"].toInt();
                                            if( fanPagesMap.contains( iFanPageLikes ) )
                                            {
                                                while( fanPagesMap.contains( iFanPageLikes ) )
                                                {
                                                    OVFB_MSG("fanPagesMap already contains this key, decreasing..." );
                                                    iFanPageLikes -=1;
                                                }
                                            }
                                            fanPagesMap[iFanPageLikes] = pagesMap[p]["id"];
                                        }
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }
                                QMapIterator<int, QString> q( fanPagesMap );
                                q.toBack();
                                while( q.hasPrevious() )
                                {
                                    q.previous();
                                    OVFB_MSG( "fanpage - fan_count: %d ; id: %s", q.key(), q.value().toUtf8().constData() );
                                    m_fanPagesIdsFound.append( q.value() );
                                }
                            }
                            else
                            {
                                OVFB_MSG( "No closing string found for fan page!" );
                            }
                        }
                        else
                        {
                            OVFB_MSG( "No opening string found for fan page!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No fan page found!" );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigFanPageRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "user_id" )
            {
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );

                    QString strJsonResponse( response );

//                    OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != "" && strJsonResponse != "false" )
                    {
                        QString strTmp = strJsonResponse.remove( 0, 7 );
                        if( strTmp.contains( "\"}" ) )
                        {
                            strTmp.chop( 2 );
                            m_strUserIdFound = strTmp;
                        }
                        else
                        {
                            OVFB_MSG( "No separator found for ID!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No ID found!" );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigUserIdRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "user_name" )
            {
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );

                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

        //            OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != "" && strJsonResponse != "false" )
                    {
                        QString strTmp = strJsonResponse.remove( 0, 9 );
                        if( strTmp.contains( "\",\"" ) )
                        {
                            QStringList tmpList = strTmp.split( "\",\"" );

                            m_strUserNameFound = tmpList.at( 0 );
                        }
                        else
                        {
                            OVFB_MSG( "No separator found for name!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No name found!" );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigUserNameRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "user_picture" )
            {
                QImage userPicture;

                if( !m_networkReply->error() )
                {
                    int iHttpStatusCodeAttribute = m_networkReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    //                OVFB_MSG( "HttpStatusCodeAttribute: %d", iHttpStatusCodeAttribute );

                    // 302 HTTP status code means a redirection was found
                    if( iHttpStatusCodeAttribute == 302 )
                    {
                        m_iNbRedirections++;
                        if( m_iNbRedirections <= m_iNbMaxRedirections )
                        {
    //                        OVFB_MSG( "Applying redirection" );
                            QString strHeader = QString( m_networkReply->header( QNetworkRequest::LocationHeader ).toString() );

    //                        m_networkReply->close();
                            m_networkReply->abort();
                            m_networkReply->deleteLater();
                            startRequest( strHeader );
                        }
                        else
                        {
                            OVFB_ERROR( "404 Error! URL not found for user picture." );
                        }
                    }
                    else
                    {
                        QByteArray response( m_networkReply->readAll() );
                        if( userPicture.loadFromData( response ) )
                        {
                            OVFB_MSG( "User Picture successfully loaded from request data!" );
                            m_userPictureFound = userPicture;
                        }
                        else
                        {
                            OVFB_ERROR( "Failed to load user picture from request data!" );
                        }

                        m_iNbRedirections = 0;
    //                    m_networkReply->close();
                        m_networkReply->abort();
                        m_networkReply->deleteLater();
                        emit( sigUserPictureRequestFinished() );
                        m_strHandleDataActionTag == "default";
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
    //                m_networkReply->close();
                    m_networkReply->abort();
                    m_networkReply->deleteLater();
                    emit( sigUserPictureRequestFinished() );
                    m_strHandleDataActionTag == "default";
                }
            }

            else if( m_strHandleDataActionTag == "friend_picture" )
            {
                QImage friendPicture;

                if( !m_networkReply->error() )
                {
                    int iHttpStatusCodeAttribute = m_networkReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    //                OVFB_MSG( "HttpStatusCodeAttribute: %d", iHttpStatusCodeAttribute );

                    // 302 HTTP status code means a redirection was found
                    if( iHttpStatusCodeAttribute == 302 )
                    {
                        m_iNbRedirections++;
                        if( m_iNbRedirections <= m_iNbMaxRedirections )
                        {
    //                        OVFB_MSG( "Applying redirection" );
                            QString strHeader = QString( m_networkReply->header( QNetworkRequest::LocationHeader ).toString() );

    //                        m_networkReply->close();
                            m_networkReply->abort();
                            m_networkReply->deleteLater();
                            startRequest( strHeader );
                        }
                        else
                        {
                            OVFB_ERROR( "404 Error! URL not found for friend picture." );
                        }
                    }
                    else
                    {
                        QByteArray response( m_networkReply->readAll() );
                        if( friendPicture.loadFromData( response ) )
                        {
                            OVFB_MSG( "Friend Picture successfully loaded from request data!" );
                            m_friendPictureFound = friendPicture;
                        }
                        else
                        {
                            OVFB_ERROR( "Failed to load user picture from request data!" );
                        }

                        m_iNbRedirections = 0;
    //                    m_networkReply->close();
                        m_networkReply->abort();
                        m_networkReply->deleteLater();
                        emit( sigFriendPictureRequestFinished() );
                        m_strHandleDataActionTag == "default";
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
    //                m_networkReply->close();
                    m_networkReply->abort();
                    m_networkReply->deleteLater();
                    emit( sigFriendPictureRequestFinished() );
                    m_strHandleDataActionTag == "default";
                }
            }

            else if( m_strHandleDataActionTag == "user_feed_post_list" )
            {
                m_userFeedPostListFound.clear();
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );
                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

    //                OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != "" && strJsonResponse != "{\"data\":[]}" )
                    {
                        if( strJsonResponse.contains( "{\"data\":[{\"id\":\"" ) )
                        {
                            QString elements = strJsonResponse;
                            elements.remove( 0, 15 );

                            bool bHasElements = false;
                            QStringList elementsList;
                            if( elements.contains( "},{\"id\":" ) )
                            {
                                bHasElements = true;
                                elementsList = elements.split( "},{\"id\":" );
                            }
                            else if( elements.contains( "}],\"paging" ))
                            {
                                bHasElements = true;
                                elementsList = elements.split( "}],\"paging" );
                            }

                            if( bHasElements )
                            {
                                int i = 0;
                                foreach( QString element, elementsList )
                                {
    //                                OVFB_MSG( "element: %s", element.toUtf8().constData() );
                                    if( element.contains( "\"message\":\"" ) )
                                    {
                                        QStringList tmpMsgList = element.split( "\"message\":\"" );
                                        QString strMsg = tmpMsgList.at( 1 );
                                        if( strMsg.contains( "\",\"" ) )
                                        {
                                            tmpMsgList = strMsg.split( "\",\"" );
                                            strMsg = tmpMsgList.at( 0 );
                                            m_userFeedPostListFound[i]["message"] = removePercentEncoding( strMsg );
                                        }
                                        else
                                        {
                                            OVFB_ERROR( "Parse error in message search!" );
                                        }

                                        if( element.contains( ",\"name\":\"" ) )
                                        {
                                            QStringList tmpMsgNameList = element.split( ",\"name\":\"" );
                                            QString strMsgName = tmpMsgNameList.at( 1 );
                                            if( strMsgName.contains( "\",\"" ) )
                                            {
                                                tmpMsgNameList = strMsgName.split( "\",\"" );
                                                strMsgName = tmpMsgNameList.at( 0 );
                                                m_userFeedPostListFound[i]["msg_name"] = strMsgName;
                                            }
                                        }

                                        if( element.contains( "\",\"" ) )
                                        {
                                            QStringList tmpPostIdList = element.split( "\",\"" );
                                            QString strPostId = tmpPostIdList.at( 0 );
                                            strPostId.remove( 0, 1 );
                                            m_userFeedPostListFound[i]["post_id"] = strPostId;
                                        }
                                        else
                                        {
                                            OVFB_ERROR( "Parse error in post id search!" );
                                        }

                                        if( element.contains( "\"from\":{" ) )
                                        {
                                            QStringList tmpUserIdList = element.split( "\"from\":{" );
                                            QString strUserId = tmpUserIdList.at( 1 );
                                            if( strUserId.contains( "\"}," ) )
                                            {
                                                tmpUserIdList = strUserId.split( "\"}," );
                                                strUserId = tmpUserIdList.at( 0 );
                                                if( strUserId.contains( "\"id\":\"" ) )
                                                {
                                                    tmpUserIdList = strUserId.split( "\"id\":\"" );
                                                    strUserId = tmpUserIdList.at( 1 );
                                                    m_userFeedPostListFound[i]["user_id"] = strUserId;
                                                }
                                            }
                                        }
                                        else
                                        {
    //                                        OVFB_MSG( "No field from!" );
                                            m_userFeedPostListFound[i]["user_id"] = "";
                                        }

                                        if( element.contains( "\"updated_time\":\"" ) )
                                        {
                                            QStringList tmpTimeList = element.split( "\"updated_time\":\"" );
                                            QString strTime = tmpTimeList.at( 1 );
                                            if( strTime.contains( "\",\"" ) )
                                            {
                                                tmpTimeList = strTime.split( "\",\"" );
                                                strTime = tmpTimeList.at( 0 );
                                                m_userFeedPostListFound[i]["time"] = strTime;
                                            }
                                            else
                                            {
                                                strTime.chop( 1 );
                                                m_userFeedPostListFound[i]["time"] = strTime;
                                            }
                                        }
                                        else
                                        {
                                            m_userFeedPostListFound[i]["updated_time"] = "";
                                        }

                                        i++;
                                    }
                                    else
                                    {
                                        OVFB_MSG( "No element containing a message!" );
                                    }
                                }
                            }
                            else
                            {
                                OVFB_ERROR( "Error parsing user feed post list: No element containing an id field!" );
                            }
                        }
                        else
                        {
                            OVFB_ERROR( "Error parsing user feed post list!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No user feed post list found!" );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigUserFeedPostListRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "user_feed_poster_name" )
            {
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );

                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

        //            OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != "" && strJsonResponse != "false" )
                    {
                        QString strTmp = strJsonResponse.remove( 0, 9 );
                        if( strTmp.contains( "\",\"" ) )
                        {
                            QStringList tmpList = strTmp.split( "\",\"" );

                            m_strUserFeedPosterNameFound = tmpList.at( 0 );
                        }
                        else
                        {
                            OVFB_MSG( "No separator found for user feed poster name!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No user feed poster name found!" );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigUserFeedPosterNameRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "user_feed_poster_picture" )
            {
                QImage userFeedPosterPicture;

                if( !m_networkReply->error() )
                {
                    int iHttpStatusCodeAttribute = m_networkReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    //                OVFB_MSG( "HttpStatusCodeAttribute: %d", iHttpStatusCodeAttribute );

                    // 302 HTTP status code means a redirection was found
                    if( iHttpStatusCodeAttribute == 302 )
                    {
                        m_iNbRedirections++;
                        if( m_iNbRedirections <= m_iNbMaxRedirections )
                        {
    //                        OVFB_MSG( "Applying redirection" );
                            QString strHeader = QString( m_networkReply->header( QNetworkRequest::LocationHeader ).toString() );

    //                        m_networkReply->close();
                            m_networkReply->abort();
                            m_networkReply->deleteLater();
                            startRequest( strHeader );
                        }
                        else
                        {
                            OVFB_ERROR( "404 Error! URL not found for user feed poster picture." );
                        }
                    }
                    else
                    {
                        QByteArray response( m_networkReply->readAll() );
                        if( userFeedPosterPicture.loadFromData( response ) )
                        {
                            OVFB_MSG( "User feed Poster Picture successfully loaded from request data!" );
                            m_UserFeedPosterPictureFound = userFeedPosterPicture;
                        }
                        else
                        {
                            OVFB_ERROR( "Failed to load user feed poster picture from request data!" );
                        }

                        m_iNbRedirections = 0;
    //                    m_networkReply->close();
                        m_networkReply->abort();
                        m_networkReply->deleteLater();
                        emit( sigUserFeedPosterPictureRequestFinished() );
                        m_strHandleDataActionTag == "default";
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
    //                m_networkReply->close();
                    m_networkReply->abort();
                    m_networkReply->deleteLater();
                    emit( sigUserFeedPosterPictureRequestFinished() );
                    m_strHandleDataActionTag == "default";
                }
            }

            else if( m_strHandleDataActionTag == "friends" )
            {
                m_friendsList.clear();
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );
                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

        //            OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != "" && strJsonResponse != "{\"data\":[]}" )
                    {
                        if( strJsonResponse.contains( "[{" ) )
                        {
                            QStringList split1 = strJsonResponse.split( "[{" );
                            if( split1.at( 1 ).contains( "}]" ) )
                            {
                                QStringList split2 = split1.at( 1 ).split( "}]" );
                                if( split2.at( 0 ).contains( "},{" ) )
                                {
                                    QStringList elements = split2.at( 0 ).split( "},{" );

                                    int i = 0;
                                    foreach( QString element, elements )
                                    {
                                        QString strTmp = element.remove( 0, 8 );
                                        if( strTmp.contains( "\",\"" ) )
                                        {
                                            QStringList tmpList = strTmp.split( "\",\"" );

                                            QString strTmpName = tmpList.at( 0 );

                                            strTmp = tmpList.at( 1 );
                                            QString strTmpId = strTmp.remove( 0, 5 );
                                            strTmpId.chop( 1 );

                                            QMap<QString, QString> tmpMap;
                                            tmpMap["name"] = strTmpName;
                                            tmpMap["id"] = strTmpId;

                                            m_friendsList[i] = tmpMap;
                                            i++;
                                        }
                                        else
                                        {
                                            OVFB_MSG( "No separator found for friend!" );
                                        }
                                    }
                                }
                                else
                                {
                                    OVFB_MSG( "No separator found for friend!" );
                                }
                            }
                            else
                            {
                                OVFB_MSG( "No closing string found for friend!" );
                            }
                        }
                        else
                        {
                            OVFB_MSG( "No opening string found for friend!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No friend found!" );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigFriendsListRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "user_likes" )
            {
                m_userLikesFound.clear();
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );
                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

    //                OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != "" )
                    {
                        if( strJsonResponse != "{\"data\":[]}" )
                        {
                            if( strJsonResponse.contains( "[{" ) )
                            {
                                QStringList split1 = strJsonResponse.split( "[{" );
                                if( split1.at( 1 ).contains( "}]" ) )
                                {
                                    QStringList split2 = split1.at( 1 ).split( "}]" );
                                    if( split2.at( 0 ).contains( "},{" ) )
                                    {
                                        QStringList elements = split2.at( 0 ).split( "},{" );

                                        QMap<QString, QString> tmpMap;
                                        QMap< int, QMap<QString, QString> > likesMap;

                                        int i = 0;
                                        foreach( QString element, elements )
                                        {
                                            QString strTmp = element.remove( 0, 8 );
                                            if( strTmp.contains( "\",\"" ) )
                                            {
                                                QStringList tmpList = strTmp.split( "\",\"" );

                                                QString strTmpName = tmpList.at( 0 );

                                                strTmp = tmpList.at( 1 );
                                                QString strTmpCategory = strTmp.remove( 0, 11 );

                                                strTmp = tmpList.at( 2 );
                                                QString strTmpId = strTmp.remove( 0, 5 );

                    //                            OVFB_MSG( "strTmpName: %s", strTmpName.toUtf8().constData() );
                    //                            OVFB_MSG( "strTmpCategory: %s", strTmpCategory.toUtf8().constData() );
                    //                            OVFB_MSG( "strTmpId: %s", strTmpId.toUtf8().constData() );
                                                if( strTmpCategory == "Film"
                                                 || strTmpCategory == "Movie"
                                                 || strTmpCategory == "Tv"
                                                 || strTmpCategory == "Television"
                                                 || strTmpCategory == "Tv show"
                                                )
                                                {
                                                    tmpMap["name"] = strTmpName;
                                                    tmpMap["category"] = strTmpCategory;
                                                    tmpMap["id"] = strTmpId;

                                                    likesMap[i] = tmpMap;
                                                    i++;
                                                }
                                            }
                                            else
                                            {
                                                OVFB_MSG( "No separator found for user likes!" );
                                            }
                                        }
                                        int iNbUserLikesFound = likesMap.size();
    //                                    OVFB_MSG( "iNbUserLikesFound: %d", iNbUserLikesFound );


                                        for( int p = 0; p < iNbUserLikesFound; p++ )
                                        {
                                            m_userLikesFound.append( likesMap[p]["id"] );
                                        }
                                    }
                                    else
                                    {
                                        OVFB_MSG( "No separator found for user likes!" );
                                    }
                                }
                                else
                                {
                                    OVFB_MSG( "No closing string found for user likes!" );
                                }
                            }
                            else
                            {
                                OVFB_MSG( "No opening string found for user likes!" );
                            }
                        }
                        else
                        {
                            OVFB_MSG( "No user likes found!" );
                        }
    //                    m_networkReply->close();
                        m_networkReply->abort();
                        m_networkReply->deleteLater();
                        emit( sigUserLikesRequestFinished() );
                        m_strHandleDataActionTag == "default";
                    }
                    else
                    {
                        OVFB_MSG( "User likes request failed, will retry." );
    //                    m_networkReply->close();
                        m_networkReply->abort();
                        m_networkReply->deleteLater();
                        emit( sigUserLikesRequestFailed() );
                        m_strHandleDataActionTag == "default";
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
    //                m_networkReply->close();
                    m_networkReply->abort();
                    m_networkReply->deleteLater();
            //        emit( sigUserLikesRequestFinished() );
                    emit( sigUserLikesRequestFailed() );
                    m_strHandleDataActionTag == "default";
                }
            }

            else if( m_strHandleDataActionTag == "fan_page_information" )
            {
                m_fanPageInformation.clear();
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );
                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

        //            OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != ""/* && strJsonResponse != "{\"data\":[]}"*/ )
                    {
                        QString strTmp = strJsonResponse.remove( 0, 2 );
                        strTmp.chop( 1 );
                        if( strTmp.contains( "\",\"" ) )
                        {
                            QStringList tmpList = strTmp.split( "\",\"" );

                            int iTmpListSize = tmpList.size();
                            for( int t = 0; t < iTmpListSize - 1; t++ )
                            {
                                if( tmpList.at( t ).contains( "\":\"" ) )
                                {
                                    QStringList element = tmpList.at( t ).split( "\":\"" );
                                    QString strKey = element.at( 0 );
                                    QString strValue = element.at( 1 );
                                    m_fanPageInformation[strKey] = strValue;
                                }
                                else
                                {
                                    OVFB_MSG( "No separator found for fan page!" );
                                }
                            }
                            if( strTmp.contains( "\"likes\":" ) )
                            {
                                QStringList tmpFanCount = strTmp.split( "\"likes\":" );
                                QString strFanCount = tmpFanCount.at( 1 );
                                m_fanPageInformation["likes"] = strFanCount;
                            }
                            else
                            {
                                OVFB_MSG( "No fan count field found in fan page!" );
                            }
                        }
                        else
                        {
                            OVFB_MSG( "No separator found for fan page!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No fan page found!" );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigFanPageInformationRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "fan_page_picture" )
            {
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );
                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

        //            OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != ""/* && strJsonResponse != "{\"data\":[]}"*/ )
                    {
                        QString strTmp = strJsonResponse.remove( 0, 2 );
                        strTmp.chop( 1 );
                        if( strTmp.contains( "\",\"" ) )
                        {
                            QStringList tmpList = strTmp.split( "\",\"" );

                            int iTmpListSize = tmpList.size();
                            for( int t = 0; t < iTmpListSize - 1; t++ )
                            {
                                if( tmpList.at( t ).contains( "\":\"" ) )
                                {
                                    QStringList element = tmpList.at( t ).split( "\":\"" );
                                    QString strKey = element.at( 0 );

                                    if( strKey == "picture" )
                                    {
                                        QString strPictureUrl = element.at( 1 );
                                        strPictureUrl.remove( "\\" );
                                        if( !strPictureUrl.isNull() && !strPictureUrl.isEmpty() )
                                        {
                                            m_strHandleDataActionTag = "fan_page_picture_extracted";

                                            startRequest( strPictureUrl );
                                        }
                                        else
                                        {
                                            OVFB_MSG( "No fan page picture found!" );
            //                                m_networkReply->close();
                                            m_networkReply->abort();
                                            m_networkReply->deleteLater();
                                            emit( sigFanPagePictureRequestFinished() );
                                            m_strHandleDataActionTag == "default";
                                        }
                                    }
                                }
                                else
                                {
                                    OVFB_MSG( "No separator found for fan page picture!" );
                                }
                            }
                            if( strTmp.contains( "\"fan_count\":" ) )
                            {
                                QStringList tmpFanCount = strTmp.split( "\"fan_count\":" );
                                QString strFanCount = tmpFanCount.at( 1 );
                                m_fanPageInformation["fan_count"] = strFanCount;
                            }
                            else
                            {
                                OVFB_MSG( "No fan_count field found!" );
                            }
                        }
                        else
                        {
                            OVFB_MSG( "No separator found for fan page picture!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No fan page found!" );
    //                    m_networkReply->close();
                        m_networkReply->abort();
                        m_networkReply->deleteLater();
                        emit( sigFanPagePictureRequestFinished() );
                        m_strHandleDataActionTag == "default";
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
    //                m_networkReply->close();
                    m_networkReply->abort();
                    m_networkReply->deleteLater();
                    emit( sigFanPagePictureRequestFinished() );
                    m_strHandleDataActionTag == "default";
                }
            }

            else if( m_strHandleDataActionTag == "fan_page_picture_extracted" )
            {
                QImage fanPagePicture;

                if( !m_networkReply->error() )
                {
                    int iHttpStatusCodeAttribute = m_networkReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    //                OVFB_MSG( "HttpStatusCodeAttribute: %d", iHttpStatusCodeAttribute );

                    // 302 HTTP status code means a redirection was found
                    if( iHttpStatusCodeAttribute == 302 )
                    {
                        m_iNbRedirections++;
                        if( m_iNbRedirections <= m_iNbMaxRedirections )
                        {
    //                        OVFB_MSG( "Applying redirection" );
                            QString strHeader = QString( m_networkReply->header( QNetworkRequest::LocationHeader ).toString() );

    //                        m_networkReply->close();
                            m_networkReply->abort();
                            m_networkReply->deleteLater();
                            startRequest( strHeader );
                        }
                        else
                        {
                            OVFB_ERROR( "404 Error! URL not found for fanpage picture." );
                        }
                    }
                    else
                    {
                        QByteArray response( m_networkReply->readAll() );
                        if( fanPagePicture.loadFromData( response ) )
                        {
                            OVFB_MSG( "Fan page Picture successfully loaded from request data!" );
                            m_fanPagePicture = fanPagePicture;
                        }
                        else
                        {
                            OVFB_ERROR( "Failed to load fan page picture from request data!" );
                        }

                        m_iNbRedirections = 0;
    //                    m_networkReply->close();
                        m_networkReply->abort();
                        m_networkReply->deleteLater();
                        emit( sigFanPagePictureRequestFinished() );
                        m_strHandleDataActionTag == "default";
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
    //                m_networkReply->close();
                    m_networkReply->abort();
                    m_networkReply->deleteLater();
                    emit( sigFanPagePictureRequestFinished() );
                    m_strHandleDataActionTag == "default";
                }
            }

            else if( m_strHandleDataActionTag == "fan_page_post_list" )
            {
                m_FanPagePostListFound.clear();
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );
                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

        //            OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != "" && strJsonResponse != "{\"data\":[]}" )
                    {
                        if( strJsonResponse.contains( "{\"data\":[{\"id\":\"" ) )
                        {
                            QString elements = strJsonResponse;
                            elements.remove( 0, 15 );

                            bool bHasElements = false;
                            QStringList elementsList;
                            if( elements.contains( "},{\"id\":" ) )
                            {
                                bHasElements = true;
                                elementsList = elements.split( "},{\"id\":" );
                            }
                            else if( elements.contains( "}],\"paging" ))
                            {
                                bHasElements = true;
                                elementsList = elements.split( "}],\"paging" );
                            }

                            if( bHasElements )
                            {
                                int i = 0;
                                foreach( QString element, elementsList )
                                {
                                    if( element.contains( "\"message\":\"" ) )
                                    {
                                        QStringList tmpMsgList = element.split( "\"message\":\"" );
                                        QString strMsg = tmpMsgList.at( 1 );
                                        if( strMsg.contains( "\",\"" ) )
                                        {
                                            tmpMsgList = strMsg.split( "\",\"" );
                                            strMsg = tmpMsgList.at( 0 );
                                            m_FanPagePostListFound[i]["message"] = strMsg;
                                        }
                                        else
                                        {
                                            OVFB_ERROR( "Parse error in message search!" );
                                        }

                                        if( element.contains( "\",\"" ) )
                                        {
                                            QStringList tmpPostIdList = element.split( "\",\"" );
                                            QString strPostId = tmpPostIdList.at( 0 );
                                            strPostId.remove( 0, 1 );
                                            m_FanPagePostListFound[i]["post_id"] = strPostId;
                                        }
                                        else
                                        {
                                            OVFB_ERROR( "Parse error in post id search!" );
                                        }

                                        if( element.contains( "\"from\":{" ) )
                                        {
                                            QStringList tmpUserIdList = element.split( "\"from\":{" );
                                            QString strUserId = tmpUserIdList.at( 1 );
                                            if( strUserId.contains( "\"}," ) )
                                            {
                                                tmpUserIdList = strUserId.split( "\"}," );
                                                strUserId = tmpUserIdList.at( 0 );
                                                if( strUserId.contains( "\"id\":\"" ) )
                                                {
                                                    tmpUserIdList = strUserId.split( "\"id\":\"" );
                                                    strUserId = tmpUserIdList.at( 1 );
                                                    m_FanPagePostListFound[i]["user_id"] = strUserId;
                                                }
                                            }
                                        }
                                        else
                                        {
    //                                        OVFB_MSG( "No field from!" );
                                            m_FanPagePostListFound[i]["user_id"] = "";
                                        }

                                        if( element.contains( "\"updated_time\":\"" ) )
                                        {
                                            QStringList tmpTimeList = element.split( "\"updated_time\":\"" );
                                            QString strTime = tmpTimeList.at( 1 );
                                            if( strTime.contains( "\",\"" ) )
                                            {
                                                tmpTimeList = strTime.split( "\",\"" );
                                                strTime = tmpTimeList.at( 0 );
                                                m_FanPagePostListFound[i]["time"] = strTime;
                                            }
                                            else
                                            {
                                                strTime.chop( 1 );
                                                m_FanPagePostListFound[i]["time"] = strTime;
                                            }
                                        }
                                        else
                                        {
                                            m_FanPagePostListFound[i]["updated_time"] = "";
                                        }

                                        i++;
                                    }
                                    else
                                    {
                                        OVFB_MSG( "No element containing a message!" );
                                    }
                                }
                            }
                            else
                            {
                                OVFB_ERROR( "Error parsing fan page post list: No element containing an 'id' field!" );
                            }
                        }
                        else
                        {
                            OVFB_ERROR( "Error parsing fan page post list!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No fan page post list found!" );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigFanPagePostListRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "fan_page_poster_name" )
            {
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );

                    QString strJsonResponse( response );
                    strJsonResponse = replaceSpecialChars( strJsonResponse );

        //            OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( strJsonResponse != "" && strJsonResponse != "false" )
                    {
                        QString strTmp = strJsonResponse.remove( 0, 9 );
                        if( strTmp.contains( "\",\"" ) )
                        {
                            QStringList tmpList = strTmp.split( "\",\"" );

                            m_strFanPagePosterNameFound = tmpList.at( 0 );
                        }
                        else
                        {
                            OVFB_MSG( "No separator found for fan page poster name!" );
                        }
                    }
                    else
                    {
                        OVFB_MSG( "No fan page poster name found!" );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigFanPagePosterNameRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "fan_page_poster_picture" )
            {
                QImage fanPagePosterPicture;

                if( !m_networkReply->error() )
                {
                    int iHttpStatusCodeAttribute = m_networkReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    //                OVFB_MSG( "HttpStatusCodeAttribute: %d", iHttpStatusCodeAttribute );

                    // 302 HTTP status code means a redirection was found
                    if( iHttpStatusCodeAttribute == 302 )
                    {
                        m_iNbRedirections++;
                        if( m_iNbRedirections <= m_iNbMaxRedirections )
                        {
    //                        OVFB_MSG( "Applying redirection" );
                            QString strHeader = QString( m_networkReply->header( QNetworkRequest::LocationHeader ).toString() );

    //                        m_networkReply->close();
                            m_networkReply->abort();
                            m_networkReply->deleteLater();
                            startRequest( strHeader );
                        }
                        else
                        {
                            OVFB_ERROR( "404 Error! URL not found for fanpage poster picture." );
                        }
                    }
                    else
                    {
                        QByteArray response( m_networkReply->readAll() );
                        if( fanPagePosterPicture.loadFromData( response ) )
                        {
                            OVFB_MSG( "Fan page Poster Picture successfully loaded from request data!" );
                            m_fanPagePosterPictureFound = fanPagePosterPicture;
                        }
                        else
                        {
                            OVFB_ERROR( "Failed to load fan page poster picture from request data!" );
                        }

                        m_iNbRedirections = 0;
    //                    m_networkReply->close();
                        m_networkReply->abort();
                        m_networkReply->deleteLater();
                        emit( sigFanPagePosterPictureRequestFinished() );
                        m_strHandleDataActionTag == "default";
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
    //                m_networkReply->close();
                    m_networkReply->abort();
                    m_networkReply->deleteLater();
                    emit( sigFanPagePosterPictureRequestFinished() );
                    m_strHandleDataActionTag == "default";
                }
            }

            else if( m_strHandleDataActionTag == "post_message" )
            {
                if( !m_networkReply->error() )
                {
                    QByteArray response( m_networkReply->readAll() );

                    QString strJsonResponse( response );
    //                strJsonResponse = replaceSpecialChars( strJsonResponse );

    //                OVFB_MSG( "JSON response: %s", strJsonResponse.toUtf8().constData() );

                    if( !strJsonResponse.isNull() && !strJsonResponse.isEmpty() )
                    {
                        QString strTmp = strJsonResponse.remove( 0, 7 );
                        strTmp.chop( 2 );

                        m_strPostId = strTmp;
                    }
                    else
                    {
                        OVFB_MSG( "Error while posting message." );
                    }
                }
                else
                {
                    OVFB_ERROR( "error: %s", m_networkReply->errorString().toUtf8().constData() );
                }
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                emit( sigPostMessageRequestFinished() );
                m_strHandleDataActionTag == "default";
            }

            else if( m_strHandleDataActionTag == "default" )
            {
    //            m_networkReply->close();
                m_networkReply->abort();
                m_networkReply->deleteLater();
                OVFB_MSG( "Nothing to do with this network request!" );
            }
    //    }
    //    else
    //    {
    //        OVFB_WARNING( "Reply doesn't match with sent reply!" );
    //    }
    }

}
