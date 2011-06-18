#include <OVAppFacebookJob.h>

#include <OVDebugCommon.h>

#include <OVDataFacebookDebug.h>

#define OVFACEBOOKCONFIGINI "OVFacebookConfig.ini"

namespace OVSchematics
{

// public

    AppFacebookJob::AppFacebookJob()
        : m_bIsFacebookSyncInProgress   ( false )
        , m_bJobRunning                 ( false )
        , m_bAuthInProgress             ( false )
        , m_bCurrentUserLikeSearch      ( false )
        , m_strHandleFailedRequestTag   ( "default" )
        , m_strCurrentUserId            ( "" )
        , m_strCurrentAccessToken       ( "" )
        , m_strCurrentFanPageId         ( "" )
        , m_strContentType              ( "" )
        , m_strContentId                ( "" )
        , m_strContentTitle             ( "" )
        , m_iWorkCounter                ( 0 )
        , m_iFriendsJobCounter          ( 0 )
        , m_iFriendNumber               ( 0 )
        , m_iNbFriends                  ( 0 )
        , m_iNbUserFeedPosters          ( 0 )
        , m_iNbFanPagePosters           ( 0 )
        , m_iFanPageNumber              ( 0 )
        , m_iNbFanPagesFound            ( 0 )
        , m_iNbLikesRequestsInProgress  ( 0 )
        , m_appDatabase                 ( NULL )
        , m_fbUser                      ( NULL )
        , m_fbUserFeedPostList          ( NULL )
        , m_fbFriendList                ( NULL )
        , m_fbFanPage                   ( NULL )
        , m_fbFanpagePostList           ( NULL )
        , m_FBInterface                 ( NULL )
    {

        this->setObjectName( "facebookThread" );
        moveToThread( this );

        QString strConfigToParse = OVFACEBOOKCONFIGINI;
        m_parser = OV_NEW(Domaine_App) OVSmartParser( strConfigToParse );

        m_strPicturesFilepath = m_parser->getConfigValue( "pictures_filepath" ).toString();
        int iMainJobFrequency = m_parser->getConfigValue( "main_job_frequency" ).toInt();
        QString strUpdateMode = m_parser->getConfigValue( "update_mode" ).toString();

        if( strUpdateMode == "on" )
        {
            m_bUpdateModeOn = true;
        }
        else
        {
            m_bUpdateModeOn = false;
        }

        OV_MSG( "Work Frequency: %d minutes", iMainJobFrequency );

        AppFacebookJob::setWakeUpTime( 60000 * iMainJobFrequency );

        m_facebookLib = OV_NEW(Domaine_App) OVData::FacebookLib;
        m_facebookLib->moveToThread( this );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigRequestFailed() ),
                    this,           SLOT(   slotHandleFailedRequest() ),
                   Qt::UniqueConnection );


        OV_CONNECT( m_facebookLib,  SIGNAL( sigUserIdRequestFinished() ),
                    this,           SLOT(   slotHandleUserIdRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigUserNameRequestFinished() ),
                    this,           SLOT(   slotHandleUserNameRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigUserPictureRequestFinished() ),
                    this,           SLOT(   slotHandleUserPictureRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigUserFeedPostListRequestFinished() ),
                    this,           SLOT(   slotHandleUserFeedPostListRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigUserFeedPosterNameRequestFinished() ),
                    this,           SLOT(   slotHandleUserFeedPostersNameRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigUserFeedPosterPictureRequestFinished() ),
                    this,           SLOT(   slotHandleUserFeedPostersPictureRequest() ),
                   Qt::UniqueConnection );


        OV_CONNECT( m_facebookLib,  SIGNAL( sigFriendsListRequestFinished() ),
                    this,           SLOT(   slotHandleFriendsListRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigFriendPictureRequestFinished() ),
                    this,           SLOT(   slotHandleFriendsPictureRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigUserLikesRequestFinished() ),
                    this,           SLOT(   slotHandleUserLikesRequest() ),
                   Qt::UniqueConnection );
        OV_CONNECT( m_facebookLib,  SIGNAL( sigUserLikesRequestFailed() ),
                    this,           SLOT(   slotHandleUserLikesFailed() ),
                   Qt::UniqueConnection );


        OV_CONNECT( m_facebookLib,  SIGNAL( sigFanPageInformationRequestFinished() ),
                    this,           SLOT(   slotHandleFanPageInformationRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigFanPagePictureRequestFinished() ),
                    this,           SLOT(   slotHandleFanPagePictureRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigFanPagePostListRequestFinished() ),
                    this,           SLOT(   slotHandleFanPagePostListRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigFanPagePosterNameRequestFinished() ),
                    this,           SLOT(   slotHandleFanPagePostersNameRequest() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_facebookLib,  SIGNAL( sigFanPagePosterPictureRequestFinished() ),
                    this,           SLOT(   slotHandleFanPagePostersPictureRequest() ),
                   Qt::UniqueConnection );


        OV_CONNECT( m_facebookLib,  SIGNAL( sigFanPageRequestFinished() ),
                    this,           SLOT(   slotHandleSearchFanPageRequest() ),
                   Qt::UniqueConnection );


        OV_CONNECT( m_facebookLib,  SIGNAL( sigPostMessageRequestFinished() ),
                    this,           SLOT(   slotHandleShareMessageRequest() ),
                   Qt::UniqueConnection );

        m_dbHandler = OV_NEW(Domaine_App) OVAppFacebookDBHandler;
//        m_dbHandler->moveToThread( this ); // add this if OVAppFacebookDBHandler inherits QObject
        m_dbHandler->setCurrentUserId( m_strCurrentUserId );
        m_dbHandler->setCurrentFanPageId( m_strCurrentFanPageId );
        m_dbHandler->setPicturesFilepath( m_strPicturesFilepath );

        m_likesTimer.setSingleShot( true );

        OV_CONNECT( &m_likesTimer,  SIGNAL( timeout() ),
                    this,           SLOT(   slotGetLikes() ),
                   Qt::UniqueConnection );
    }

    AppFacebookJob::~AppFacebookJob()
    {
        if( m_fbFanpagePostList && !m_fbFanpagePostList->destroying() )
        {
            m_fbFanpagePostList->setFanpage( NULL );
            foreach( OVData::FbFanpagePost * fbFanpagePost, m_fbFanpagePostList->posts() )
            {
                fbFanpagePost = NULL;
            }
            OV_DELETE( m_fbFanpagePostList );
        }
        if( m_fbFanPage && !m_fbFanPage->destroying() )
        {
            m_fbFanPage->setUserList( NULL );
            m_fbFanPage->setFriendList( NULL );
            m_fbFanPage->setPostList( NULL );
            OV_DELETE( m_fbFanPage );
        }
        if( m_fbFriendList && !m_fbFriendList->destroying() )
        {
            m_fbFriendList->setUser( NULL );
            foreach( OVData::FbFriend * fbFriend, m_fbFriendList->friends() )
            {
                fbFriend = NULL;
            }
            OV_DELETE( m_fbFriendList );
        }
        if( m_fbUserFeedPostList && !m_fbUserFeedPostList->destroying() )
        {
            m_fbUserFeedPostList->setUser( NULL );
            foreach( OVData::FbUserfeedpost * fbUserFeedPost, m_fbUserFeedPostList->posts() )
            {
                fbUserFeedPost = NULL;
            }
            OV_DELETE( m_fbUserFeedPostList );
        }
        if( m_fbUser && !m_fbUser->destroying() )
        {
            m_fbUser->setFriendList( NULL );
            m_fbUser->setUserfeedpostList( NULL );
            OV_DELETE( m_fbUser );
        }

        foreach( OVData::FbFanpage* fbFanpage, m_fbFanPageQList )
        {
            if( fbFanpage && !fbFanpage->destroying() )
            {
                fbFanpage->setUserList( NULL );
                fbFanpage->setFriendList( NULL );
                fbFanpage->setPostList( NULL );
                OV_DELETE( fbFanpage );
            }
        }

//        delete m_FBInterface;
        OV_DELETE( m_dbHandler );

//        SAFE_OV_DELETE( m_appDatabase );

        // TODO : test this (deleteLater instead of delete)
//        delete m_facebookLib;
        m_facebookLib->deleteLater();       // TODO: CHECK IF DIFFERENT FROM DELETE

        OV_DELETE( m_parser );
    }



    void AppFacebookJob::connectToDatabase()
    {
        m_appDatabase = AppPluginJob::getAppDatabase();
        m_dbHandler->setAppDatabase( m_appDatabase );
        m_FBInterface->setAppDatabase( m_appDatabase );
        m_FBInterface->setCurrentUserId( m_strCurrentUserId );
        m_FBInterface->setCurrentFanPageId( m_strCurrentFanPageId );
    }

    void AppFacebookJob::setInterface( OVAppFacebookInterface* _fbInterface )
    {
        m_FBInterface = _fbInterface;
    }


// private

    void AppFacebookJob::work()
    {
        OVFB_IMPORTANT_MSG( "Starting Facebook Work" );

        if( !m_bUpdateModeOn )
        {
            OVFB_MSG( "Should update current user information. But won't do it for now." );
//            bool bProcessJob = false;
//            OVData::FbUser* fbUser = m_dbHandler->getUserFromDB( m_strCurrentUserId );
//            if( !fbUser )
//            {
//                bProcessJob = true;
//            }
//            else
//            {
//                OVData::FbUserfeedpostList* fbUserFeedPostList = m_dbHandler->getUserFeedPostListFromDB( m_strCurrentUserId );
//                if( !fbUserFeedPostList )
//                {
//                    bProcessJob = true;
//                }
//            }
//            if( bProcessJob )
//            {
//                OVFB_MSG( "Adding job 'user_news_feed' to jobs queue." );
//                m_jobsQueue.append( "user_news_feed" );

//                OVFB_MSG( "Adding job 'friendlist' to jobs queue." );
//                m_jobsQueue.append( "friendlist" );
//            }
//            else
//            {
//                OVFB_MSG( "Current Facebook user already in database, will be updated later." );
//            }
        }
        else
        {
            OVFB_MSG( "Adding job 'user_news_feed' to jobs queue." );
            m_jobsQueue.append( "user_news_feed" );

//            OVFB_MSG( "m_iFriendsJobCounter: %d - m_iWorkCounter: %d - m_iWorkCounter/10: %d", m_iFriendsJobCounter, m_iWorkCounter, m_iWorkCounter / 10 );
            // Adding job friend list 1 time every 6 occurences of Work (if work is run every 30min, friendlist job is run every 3 hours)
            if( m_iFriendsJobCounter == m_iWorkCounter / 6 )
            {
                OVFB_MSG( "Adding job 'friendlist' to jobs queue." );
                m_jobsQueue.append( "friendlist" );
                m_iFriendsJobCounter++;
            }
            m_iWorkCounter++;

//            OVFB_MSG( "Adding job 'fanpage' to jobs queue." );
//            m_jobsQueue.append( "fanpage" );
        }
        processJobsQueue();
    }

    void AppFacebookJob::slotMessageFromInterfaceReceived( int _iID, QVariant _message )
    {
        OVFB_MSG( "Message received from Interface (%d)", _iID );
        switch( _iID )
        {
        case 0 :
        {
            // Authentication process finished
            m_bAuthInProgress = false;

            // access token
            QStringList socialEmailAccessToken( _message.toStringList() );
            m_strCurrentUserEmail = socialEmailAccessToken.at( 0 );
            m_strCurrentUserPwd = socialEmailAccessToken.at( 1 );
            m_strCurrentAccessToken = socialEmailAccessToken.at( 2 );
            OVFB_MSG( "email & access_token received from interface - email: %s, access_token: %s", m_strCurrentUserEmail.toUtf8().constData(), m_strCurrentAccessToken.toUtf8().constData() );
            if( m_strCurrentUserEmail.isNull() || m_strCurrentUserEmail.isEmpty() || m_strCurrentUserPwd.isNull() || m_strCurrentUserPwd.isEmpty() || m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
            {
                OVFB_WARNING( "email/pwd or access token not received from interface!" );
            }
            else
            {
                // Sending message to UI telling we're connected to Facebook
                OVFB_MSG( "Sending message to UI - authentication success" );
                bool bIsFacebookOn = true;
                emit sigSendMessageToUI( 1, QVariant().fromValue( bIsFacebookOn ) );

                m_strCurrentFanPageId = "";
                m_dbHandler->setCurrentFanPageId( m_strCurrentFanPageId );
                m_FBInterface->setCurrentFanPageId( m_strCurrentFanPageId );


                // Prepending jobs at beginning of the queue (in reversed way), as those jobs have to be run first

                OVFB_MSG( "Adding job 'friendlist' to jobs queue." );
                m_jobsQueue.prepend( "friendlist" );

                OVFB_MSG( "Adding job 'user_news_feed' to jobs queue." );
                m_jobsQueue.prepend( "user_news_feed" );

                OVFB_MSG( "Adding job 'user_id' to jobs queue." );
                m_jobsQueue.prepend( "user_id|" + m_strCurrentAccessToken );



//                OVFB_MSG( "Adding job 'user_id' to jobs queue." );
//                m_jobsQueue.append( "user_id|" + m_strCurrentAccessToken );

//                OVFB_MSG( "Adding job 'user_news_feed' to jobs queue." );
//                m_jobsQueue.append( "user_news_feed" );

//                OVFB_MSG( "Adding job 'friendlist' to jobs queue." );
//                m_jobsQueue.append( "friendlist" );

                processJobsQueue();
            }
        }
            break;
        case 1:
        {
            // fanpage
            QString strContent = _message.toString().trimmed();
            OVFB_MSG( "Adding job 'search_fanpage' to jobs queue." );
            m_jobsQueue.append( "search_fanpage|" + strContent );
            if( !m_bAuthInProgress )
            {
                processJobsQueue();
            }
        }
            break;
        case 2:
        {
            // Authentication process started
            m_bAuthInProgress = true;
            sendSyncStatus( true );
        }
            break;
        case 3:
        {
            // Social Browser OAuth action failed
            m_bAuthInProgress = false;
            OVFB_MSG( "Sending message to UI - error in facebook authentication" );
            emit sigSendMessageToUI( 2, _message );
        }
            break;
        case 4:
        {
            // Social Browser Like action failed
            OVFB_MSG( "Sending message to UI - error while making iLike" );
            emit sigSendMessageToUI( 3, _message );
        }
            break;
        case 5:
        {
            // Social Browser Like action successful

            OVData::FbFanpage* fbFanPage = OV_NEW( Domaine_App ) OVData::FbFanpage();
            fbFanPage->setPage_id( m_strCurrentFanPageId );

            if( !m_dbHandler->addFanPageToDB( fbFanPage ) )
            {
                OVFB_ERROR( "Error while adding fan page to database!" );
            }

            if( !m_fbUser )
            {
                OV_MSG( "Creating instance of fbUser..." );
                m_fbUser = OV_NEW( Domaine_App ) OVData::FbUser();
                m_fbUser->user_id() = m_strCurrentUserId;
                if( !m_dbHandler->addUserToDB( m_fbUser ) )
                {
                    OVFB_ERROR( "Error while adding user to database!" );
                }
            }

            OV_MSG( "fbUser id: %s!", m_fbUser->user_id().toUtf8().constData() );

            m_dbHandler->addUserToFanPageUserListToDB( fbFanPage, m_fbUser );
            m_fbFanPageQList.append( fbFanPage );

            OVFB_MSG( "Sending message to UI - like successful" );
            emit sigSendMessageToUI( 5, _message );
        }
            break;
        case 6:
        {
            // Social Browser unlike action successful

            foreach ( QString strFanPageId, m_fanPagesToUnlike )
            {
                if( !m_dbHandler->removeFanPageLike( m_strCurrentUserId, strFanPageId ) )
                {
                    OV_MSG( "Could not delete like in DB for fanpage %s!", strFanPageId.toUtf8().constData() );
                }
            }
            OVFB_MSG( "Sending message to UI - unlike successful" );
            emit sigSendMessageToUI( 6, _message );
        }
            break;
        default:
            break;
        };
    }

    void AppFacebookJob::getMessageFromUI( int _iID, QVariant _messageValue )
    {
        OVFB_MSG( "Message received from UI (%d)", _iID );
        switch( _iID )
        {
        case 0 :
        {
            OVFB_MSG( "case 0" );
            // Retrieve user's friends likes and user's like status for a content
            QStringList contentMessage = _messageValue.toStringList();
            if( contentMessage.at( 0 ).isNull() || contentMessage.at( 0 ).isEmpty() )
            {
                OVFB_ERROR( "content type empty!" );
            }
            else
            {
                if( contentMessage.at( 1 ).isNull() || contentMessage.at( 1 ).isEmpty() )
                {
                    OVFB_ERROR( "content ID empty!" );
                }
                else
                {
                    if( contentMessage.at( 2 ).isNull() || contentMessage.at( 2 ).isEmpty() )
                    {
                        OVFB_ERROR( "content title empty!" );
                    }
                    else
                    {
                        OVFB_MSG( "contentMessage.at( 1 ).trimmed(): %s", contentMessage.at( 1 ).trimmed().toUtf8().constData() );
                        OVFB_MSG( "m_strContentId: %s", m_strContentId.toUtf8().constData() );
                        OVFB_MSG( "m_strContentTitle: %s", m_strContentTitle.toUtf8().constData() );
                        if( contentMessage.at( 1 ) != m_strContentId )
//                        if( contentMessage.at( 2 ).trimmed() != m_strContentTitle )
                        {
                            m_iNbLikesRequestsInProgress += 1;

                            // Search fan page corresponding to _messageValue and search likes
                            m_socialMessage.m_iLikeNumber = 0;
                            m_socialMessage.m_listFriendsLike.clear();
                            m_socialMessage.m_strTitle = contentMessage.at( 2 );     // TODO : si on fait une autre demande de likes alors que la 1ere n'est pas finie,
                                                                                                // la valeur de m_socialMessage.m_strTitle est écrasée !!!!! A CHECKER
                            m_strContentType = contentMessage.at( 0 );
                            m_strContentId = contentMessage.at( 1 );
                            m_strContentTitle = contentMessage.at( 2 ).trimmed();
                            OVFB_MSG( "Type: get likes for content: (type:%s|id:%s|title:%s)", m_strContentId.toUtf8().constData(), m_strContentTitle.toUtf8().constData(), m_strContentType.toUtf8().constData() );

                            if( isContentInDB( m_strContentId ) )
                            {
                                stopLikesTimer();

                                OVFB_MSG( "Adding job 'fbdb__content_user_like_status' to jobs queue." );
                                m_jobsQueue.append( "fbdb__content_user_like_status|" + m_strContentId );

                                OVFB_MSG( "Adding job 'fbdb__content_friends_likes' to jobs queue." );
                                m_jobsQueue.append( "fbdb__content_friends_likes|" + m_strContentId );

                                if( !m_bAuthInProgress )
                                {
                                    processJobsQueue();
                                }
                            }
                            else
                            {
                                getLikesForContentOverNetwork();
                            }
                        }
                        else
                        {
                            if( m_iNbLikesRequestsInProgress == 0 )
                            {
                                OVFB_MSG( "Sending message to UI - content ID: %s - content title: %s", m_strContentId.toUtf8().constData(), m_strContentTitle.toUtf8().constData() );
                                m_strContentId = "";
                                m_strContentTitle = "";
                                emit sigSendMessageToUI( 8, m_strContentId );
                            }
                        }
                    }
                }
            }
        }
            break;
        case 1:
        {
            // Send comment to FB
            QStringList share( _messageValue.toStringList() );
            QString strContent = share.at( 0 ).trimmed();
            QString strMessage = share.at( 1 ).trimmed();
            OVFB_MSG( "Type: share - Content: %s ; Message: %s", strContent.toUtf8().constData(), strMessage.toUtf8().constData() );
            shareMessage( strContent, strMessage );
        }
            break;
        case 2:
        {
            // Make iLike
            m_strContentTitle = _messageValue.toString().trimmed();
            OVFB_MSG( "Type: make iLike for content: (%s)", m_strContentTitle.toUtf8().constData() );
            makeLikeOnContent();
        }
            break;
        case 3:
        {
            // Change profile
            QStringList loginPassword( _messageValue.toStringList() );
            m_strCurrentUserEmail = loginPassword.at( 0 ).trimmed();
            m_strCurrentUserPwd = loginPassword.at( 1 ).trimmed();
            OVFB_MSG( "Type: login_password - Login: %s ; Password: %s", m_strCurrentUserEmail.toUtf8().constData(), m_strCurrentUserPwd.toUtf8().constData() );

            m_strCurrentFanPageId = "";
            m_dbHandler->setCurrentFanPageId( m_strCurrentFanPageId );
            m_FBInterface->setCurrentFanPageId( m_strCurrentFanPageId );

            OVData::FbUser* fbUser = m_dbHandler->getUserWithEmailFromDB( m_strCurrentUserEmail );
            if( !fbUser )
            {
                OVFB_MSG( "No user found on facebook plugin with email %s", m_strCurrentUserEmail.toUtf8().constData() );
                m_strCurrentUserId = "";
                m_strCurrentAccessToken = "";

                m_FBInterface->setCurrentUserId( m_strCurrentUserId );
                m_dbHandler->setCurrentUserId( m_strCurrentUserId );

                sendMessageToInterface( -1 );

                // Sending message to UI telling we're not connected to Facebook
                OVFB_MSG( "Sending message to UI - turn off facebook plugin" );
                bool bIsFacebookOn = false;
                emit sigSendMessageToUI( 1, QVariant().fromValue( bIsFacebookOn ) );
            }
            else
            {
                OVFB_MSG( "Switching facebook plugin to user with email %s", m_strCurrentUserEmail.toUtf8().constData() );
                m_strCurrentUserId = fbUser->user_id();
                m_strCurrentAccessToken = fbUser->access_token();

                m_FBInterface->setCurrentUserId( m_strCurrentUserId );
                m_dbHandler->setCurrentUserId( m_strCurrentUserId );

                sendMessageToInterface( -1 );
                sendMessageToInterface( 0 );
//                sendMessageToInterface( 1 );
                sendMessageToInterface( 4 );

                // Sending message to UI telling we're connected to Facebook
                OVFB_MSG( "Sending message to UI - switched facebook user" );
                bool bIsFacebookOn = true;
                emit sigSendMessageToUI( 1, QVariant().fromValue( bIsFacebookOn ) );
            }
        }
            break;
        case 4:
        {
            // Profile without Facebook credentials
            m_strCurrentUserId = "";
            m_strCurrentUserEmail = "";
            m_strCurrentUserPwd = "";
            m_strCurrentFanPageId = "";
            m_strCurrentAccessToken = "";

            m_FBInterface->setCurrentUserId( m_strCurrentUserId );
            m_dbHandler->setCurrentUserId( m_strCurrentUserId );

            sendMessageToInterface( -1 );
        }
            break;
        case 5 :
        {
            // Retrieve user'like status for a content
            QStringList contentMessage = _messageValue.toStringList();
            if( contentMessage.at( 0 ).isNull() || contentMessage.at( 0 ).isEmpty() )
            {
                OVFB_ERROR( "content type empty!" );
            }
            else
            {
                if( contentMessage.at( 1 ).isNull() || contentMessage.at( 1 ).isEmpty() )
                {
                    OVFB_ERROR( "content ID empty!" );
                }
                else
                {
                    if( contentMessage.at( 2 ).isNull() || contentMessage.at( 2 ).isEmpty() )
                    {
                        OVFB_ERROR( "content title empty!" );
                    }
                    else
                    {
                        if( contentMessage.at( 1 ) != m_strContentId )
                        {
                            m_strContentType = contentMessage.at( 0 );
                            m_strContentId = contentMessage.at( 1 );
                            m_strContentTitle = contentMessage.at( 2 ).trimmed();
                            OVFB_MSG( "Type: get user like status for content: (type:%s|id:%s|title:%s)", m_strContentId.toUtf8().constData(), m_strContentTitle.toUtf8().constData(), m_strContentType.toUtf8().constData() );

                            if( isContentInDB( m_strContentId ) )
                            {
                                stopLikesTimer();

                                OVFB_MSG( "Adding job 'fbdb__content_user_like_status' to jobs queue." );
                                m_jobsQueue.append( "fbdb__content_user_like_status|" + m_strContentId );
                                if( !m_bAuthInProgress )
                                {
                                    processJobsQueue();
                                }
                            }
                            else
                            {
                                m_iNbLikesRequestsInProgress += 1;

                                getLikesForContentOverNetwork();
                            }
                        }
                    }
                }
            }
        }
            break;
        case 6:
        {
            // Make unlike
            m_strContentTitle = _messageValue.toString().trimmed();
            OVFB_MSG( "Type: make unlike for content: (%s)", m_strContentTitle.toUtf8().constData() );
            makeUnlikeOnContent();
        }
            break;
        default:
            break;
        };
    }

    bool AppFacebookJob::isContentInDB( QString _strContentId )
    {
        OVFB_MSG( "Looking for content in DB. (%s)", _strContentId.toUtf8().constData() );
        OVData::FbContent* fbContent = m_dbHandler->getContentFromDB( _strContentId );
        if( !fbContent )
        {
            OVFB_MSG( "Content not found in DB" );
            return false;
        }
        OVFB_MSG( "Content found in DB!" );
        return true;
    }

    void AppFacebookJob::checkIfCurrentUserLikesContent( QString _strContentId )
    {
        OVData::FbContent* fbContent = m_dbHandler->getContentFromDB( _strContentId );

        bool bUserIsFanOfContent = m_dbHandler->checkContentLike( m_strCurrentUserId, _strContentId );

        OVFB_MSG( "Sending message to UI - content ID: %s - content title: %s - current user likes content: %s", m_strContentId.toUtf8().constData(), m_strContentTitle.toUtf8().constData(), ( bUserIsFanOfContent ) ? "YES" : "NO" );
        QStringList contentToUI;
        contentToUI.append( m_strContentId );
        contentToUI.append( ( bUserIsFanOfContent ) ? "YES" : "NO" );
        m_strContentId = "";
        m_strContentTitle = "";
        emit sigSendMessageToUI( 4, contentToUI );

        m_iNbFanPagesFound = fbContent->fanpageList().size();
        m_fanPagesFound.clear();

        foreach( OVData::FbFanpage* fbFanpage, fbContent->fanpageList() )
        {
            m_fanPagesFound.append( fbFanpage->page_id() );
        }
    }

    void AppFacebookJob::getLikesForContentOverNetwork()
    {
        int iTimeToWait = 2000;
        OVFB_MSG( "Waiting %d ms before getting likes for content: %s", iTimeToWait, m_strContentTitle.toUtf8().constData() );

        stopLikesTimer();

        OVFB_MSG( "Starting new timer." );
        m_likesTimer.start( iTimeToWait );
    }

    void AppFacebookJob::stopLikesTimer()
    {
        if( m_likesTimer.isActive() )
        {
            OVFB_MSG( "Timer already active. Stopping old timer." );
            m_likesTimer.stop();
            m_iNbLikesRequestsInProgress -= 1;
        }
    }

    void AppFacebookJob::getLikesForContent()
    {
        OVFB_MSG( "Getting likes for content (%s)", m_strContentTitle.toUtf8().constData() );
        QString strFanPageId;
        QStringList friendsIds;
        QStringList pics;
        int iNbPicsToSend = 5;  // we send only 5 pictures to Schem. TODO Schem: receive this number from Schem
        int iPicsCounter = 0;
        for( int i = 0; i < m_iNbFanPagesFound; i++ )
        {
            strFanPageId = m_fanPagesFound.at( i );
            OVFB_MSG( "Getting friends likes of user %s for fanpage %s", m_strCurrentUserId.toUtf8().constData(), strFanPageId.toUtf8().constData() );

            QList<OVData::FbFriend *> fbFriendList = m_dbHandler->getFanPageFriendListForUserFromDB( strFanPageId, m_strCurrentUserId );


            if( fbFriendList.isEmpty() )
            {
                OVFB_MSG( "No friend is fan of fan page %s", strFanPageId.toUtf8().constData() );
            }
            else
            {
                foreach( OVData::FbFriend* fbFriend, fbFriendList )
                {
                    if( !fbFriend->user_id().isNull() && !fbFriend->user_id().isEmpty() )
                    {
                        QString strFriendId = fbFriend->user_id();
                        if( !friendsIds.contains( strFriendId ) )
                        {
                            friendsIds.append( strFriendId );
                            if( iPicsCounter < iNbPicsToSend )
                            {
                                if( !fbFriend->picture().isNull() && !fbFriend->picture().isEmpty() )
                                {
                                    QString strPicPath = fbFriend->picture();
                                    OVFB_MSG( "Picture path for fan: %s", strPicPath.toUtf8().constData() );
                                    pics.append( strPicPath );
                                    iPicsCounter++;
                                }
                                else
                                {
                                    OVFB_WARNING( "picture not found in database for friend %s", fbFriend->user_id().toUtf8().constData() );
                                }
                            }
                        }
                    }
                    else
                    {
                        OVFB_WARNING( "ID not found in database for friend %s", fbFriend->user_id().toUtf8().constData() );
                    }
                }
            }
        }
        int iFanCount = friendsIds.size();
        OVFB_MSG( "%d fan(s) found", iFanCount );

        // TODO - FIXME : put this in a function
        m_socialMessage.m_iLikeNumber = iFanCount;
        m_socialMessage.m_listFriendsLike = pics;

        m_iNbLikesRequestsInProgress -= 1;

        // Sending only last social message to UI
        if( m_iNbLikesRequestsInProgress == 0 )
        {
            m_strContentId = "";
            m_strContentTitle = "";
            OVFB_MSG( "Sending message to UI - content title: %s; nb likes: %d", m_socialMessage.m_strTitle.toUtf8().constData(), m_socialMessage.m_iLikeNumber );
            emit sigSendMessageToUI( 0, QVariant().fromValue( m_socialMessage ) );
        }
    }

    void AppFacebookJob::slotGetLikes()
    {
        OVFB_MSG( "Adding job 'fbapi__content_likes' to jobs queue." );
        m_jobsQueue.append( "fbapi__content_likes|" + m_strContentTitle );
        if( !m_bAuthInProgress )
        {
            processJobsQueue();
        }
    }

    void AppFacebookJob::makeLikeOnContent()
    {
        OVFB_MSG( "Adding job 'like_content' to jobs queue." );
        m_jobsQueue.append( "like_content|" + m_strContentTitle );
        if( !m_bAuthInProgress )
        {
            processJobsQueue();
        }
    }

    void AppFacebookJob::makeUnlikeOnContent()
    {
        OVFB_MSG( "Adding job 'unlike_content' to jobs queue." );
        m_jobsQueue.append( "unlike_content|" + m_strContentTitle );
        if( !m_bAuthInProgress )
        {
            processJobsQueue();
        }
    }

    void AppFacebookJob::shareMessage( QString _strContent, QString _strMessage )
    {
        OVFB_MSG( "Adding job 'share_message' to jobs queue." );
        m_jobsQueue.append( "share_message|" + _strContent + "|" + _strMessage );
        if( !m_bAuthInProgress )
        {
            processJobsQueue();
        }
    }

    void AppFacebookJob::setJobTerminated( int _iMessageForInterface, QString _strJobName )
    {
        if( m_bJobRunning )
        {
            OVFB_IMPORTANT_MSG( "Facebook Job terminated (%s).", _strJobName.toUtf8().constData() );
            m_bJobRunning = false;
            if( _iMessageForInterface == 4 )
            {
                // Sending sync status not running to UI if job 'friendlist' (4) is done
                sendSyncStatus( false );
            }
            if( _iMessageForInterface > -1 )
            {
                AppPluginJob::sendMessageToInterface( _iMessageForInterface );
            }
            processJobsQueue();
        }
    }

    void AppFacebookJob::processJobsQueue()
    {
        OVFB_IMPORTANT_MSG( "Processing jobs queue..." );
        if( m_bJobRunning )
        {
            OVFB_IMPORTANT_MSG( "A Facebook job is already running, waiting for it to end..." );
        }
        else
        {
            if( !m_jobsQueue.isEmpty() )
            {
                int iNbJobs = m_jobsQueue.size();
                OVFB_IMPORTANT_MSG( "%d job(s) in queue:", iNbJobs );
                int i = 0;
                foreach( QString strJob, m_jobsQueue )
                {
                    OVFB_MSG( "(%d) %s", i, strJob.toUtf8().constData() );
                    i++;
                }

                QString strNextJob = m_jobsQueue.first();
                OVFB_IMPORTANT_MSG( "Next job: %s", strNextJob.toUtf8().constData() );
                m_jobsQueue.removeFirst();

                if( strNextJob.contains( "user_id|" ) )
                {
                    m_strCurrentJob = "user_id";
                    strNextJob.remove( 0, 8 );
                    if( strNextJob.isNull() || strNextJob.isEmpty() )
                    {
                        OVFB_WARNING( "No access_token. Cannot retrieve user ID!" );
                        processJobsQueue();
                    }
                    else
                    {
                        m_strCurrentAccessToken = strNextJob;
                        OVFB_IMPORTANT_MSG( "Starting Facebook Job: user ID - access_token: %s", m_strCurrentAccessToken.toUtf8().constData() );
                        m_bJobRunning = true;
                        requestUserId();
                    }
                }
                else if( strNextJob == "user_news_feed" )
                {
                    OVFB_IMPORTANT_MSG( "Starting Facebook Job: user_news_feed" );
                    m_bJobRunning = true;
                    m_strCurrentJob = "user_news_feed";
                    startRequestsForCategoryUserNewsFeed();
                }
                else if( strNextJob == "friendlist" )
                {
                    OVFB_IMPORTANT_MSG( "Starting Facebook Job: friendlist" );
                    m_bJobRunning = true;
                    m_strCurrentJob = "friendlist";
                    startRequestsForCategoryFriendList();
                }
                else if( strNextJob == "fanpage" )
                {
                    OVFB_IMPORTANT_MSG( "Starting Facebook Job: fanpage" );
                    m_bJobRunning = true;
                    m_strCurrentJob = "fanpage";
                    startRequestsForCategoryFanPage();
                }
                else if( strNextJob.contains( "search_fanpage" ) )
                {
                    m_strCurrentJob = "search_fanpage";
                    QStringList searchFanPage = strNextJob.split( "|" );
                    if( searchFanPage.at( 1 ).isNull() || searchFanPage.at( 1 ).isEmpty() )
                    {
                        OVFB_WARNING( "No content. Cannot search fanpage!" );
                        processJobsQueue();
                    }
                    else
                    {
                        QString strContent = searchFanPage.at( 1 );
                        OVFB_IMPORTANT_MSG( "Starting Facebook Job: Search fan page: %s", strContent.toUtf8().constData() );
                        m_bJobRunning = true;
                        requestSearchFanPage( strContent );
                    }
                }
                else if( strNextJob.contains( "fbdb__content_user_like_status" ) )
                {
                    m_strCurrentJob = "fbdb__content_user_like_status";
                    QStringList contentUserLike = strNextJob.split( "|" );
                    if( contentUserLike.at( 1 ).isNull() || contentUserLike.at( 1 ).isEmpty() )
                    {
                        OVFB_WARNING( "No content. Cannot retrieve current user likeness!" );
                        processJobsQueue();
                    }
                    else
                    {
                        QString strContentId = contentUserLike.at( 1 );
                        OVFB_IMPORTANT_MSG( "Starting Facebook Job: Get current user likeness for content: %s", strContentId.toUtf8().constData() );
                        m_bJobRunning = true;
                        checkIfCurrentUserLikesContent( strContentId );

                        setJobTerminated( -1, "fbdb__content_user_like_status" );
                    }
                }
                else if( strNextJob.contains( "fbdb__content_friends_likes" ) )
                {
                    m_strCurrentJob = "fbdb__content_friends_likes";
                    QStringList contentUserLike = strNextJob.split( "|" );
                    if( contentUserLike.at( 1 ).isNull() || contentUserLike.at( 1 ).isEmpty() )
                    {
                        OVFB_WARNING( "No content. Cannot retrieve current user friends likes!" );
                        processJobsQueue();
                    }
                    else
                    {
                        QString strContentId = contentUserLike.at( 1 );
                        OVFB_IMPORTANT_MSG( "Starting Facebook Job: Get current user friends likes for content: %s", strContentId.toUtf8().constData() );
                        m_bJobRunning = true;
                        m_strContentId = strContentId;
                        getLikesForContent();

                        setJobTerminated( -1, "fbdb__content_friends_likes" );
                    }
                }
                else if( strNextJob.contains( "fbapi__content_likes" ) )
                {
                    m_strCurrentJob = "fbapi__content_likes";
                    QStringList contentLikes = strNextJob.split( "|" );
                    if( contentLikes.at( 1 ).isNull() || contentLikes.at( 1 ).isEmpty() )
                    {
                        OVFB_WARNING( "No content. Cannot retrieve current user friends likes!" );
                        processJobsQueue();
                    }
                    else
                    {
                        QString strContent = contentLikes.at( 1 );
                        OVFB_IMPORTANT_MSG( "Starting Facebook Job: Get likes for content: %s", strContent.toUtf8().constData() );
                        m_bJobRunning = true;
//                        m_socialMessage.m_strTitle = strContent;
                        requestSearchFanPage( strContent );
                    }
                }
                else if( strNextJob.contains( "unlike_content" ) )
                {
                    m_strCurrentJob = "unlike_content";
                    QStringList unlikeContent = strNextJob.split( "|" );
                    if( unlikeContent.at( 1 ).isNull() || unlikeContent.at( 1 ).isEmpty() )
                    {
                        OVFB_WARNING( "No content. Cannot make unlike!" );
                        processJobsQueue();
                    }
                    else
                    {
                        QString strContent = unlikeContent.at( 1 );
                        OVFB_IMPORTANT_MSG( "Starting Facebook Job: Make unlike for content: %s", strContent.toUtf8().constData() );
                        m_bJobRunning = true;
                        requestSearchFanPage( strContent );
                    }
                }
                else if( strNextJob.contains( "like_content" ) )
                {
                    m_strCurrentJob = "like_content";
                    QStringList likeContent = strNextJob.split( "|" );
                    if( likeContent.at( 1 ).isNull() || likeContent.at( 1 ).isEmpty() )
                    {
                        OVFB_WARNING( "No content. Cannot make like!" );
                        processJobsQueue();
                    }
                    else
                    {
                        QString strContent = likeContent.at( 1 );
                        OVFB_IMPORTANT_MSG( "Starting Facebook Job: Make like for content: %s", strContent.toUtf8().constData() );
                        m_bJobRunning = true;
                        requestSearchFanPage( strContent );
                    }
                }
                else if( strNextJob.contains( "share_message" ) )
                {
                    m_strCurrentJob = "share_message";
                    QStringList share = strNextJob.split( "|" );
                    if( share.at( 1 ).isNull() || share.at( 1 ).isEmpty() )
                    {
                        OVFB_WARNING( "No content. Cannot share message!" );
                        processJobsQueue();
                    }
                    else if( share.at( 2 ).isNull() || share.at( 2 ).isEmpty() )
                    {
                        OVFB_WARNING( "No message. Cannot share message!" );
                        processJobsQueue();
                    }
                    else
                    {
                        m_strContentToShare = share.at( 1 );
                        m_strMessageToShare = share.at( 2 );
                        OVFB_IMPORTANT_MSG( "Starting Facebook Job: Share message for content: %s - message: %s", m_strContentToShare.toUtf8().constData(), m_strMessageToShare.toUtf8().constData() );
                        m_bJobRunning = true;

                        requestSearchFanPage( m_strContentToShare );
                    }
                }
                else
                {
                    OVFB_WARNING( "next job not valid: %s", strNextJob.toUtf8().constData() );
                    processJobsQueue();
                }
            }
            else
            {
                OVFB_IMPORTANT_MSG( "No more jobs in queue. Jobs queue terminated." );
            }
        }
    }

    void AppFacebookJob::sendSyncStatus( bool _bIsSyncRunning )
    {
        // Sending message to UI telling Facebook Sync is running or not
        OVFB_MSG( "Sending message to UI - Facebook Sync %s running", ( _bIsSyncRunning ) ? "is" : "is not" );
        m_bIsFacebookSyncInProgress = _bIsSyncRunning;
        emit sigSendMessageToUI( 7, QVariant().fromValue( m_bIsFacebookSyncInProgress ) );
    }

    void AppFacebookJob::startRequestsForCategoryUserNewsFeed()
    {
        if( m_strCurrentUserId.isNull() || m_strCurrentUserId.isEmpty() )
        {
            OVFB_WARNING( "current user ID null or empty, cannot start user news feed requests!" );
            setJobTerminated( 0, "user_news_feed" );
        }
        else
        {
            if( !m_fbUser )
            {
                OVFB_MSG( "Creating OVData::FbUser object" );
                m_fbUser = OV_NEW(Domaine_App) OVData::FbUser();
            }

            OVFB_MSG( "Starting requests for user %s", m_strCurrentUserId.toUtf8().constData() );

            m_fbUser->setUser_id( m_strCurrentUserId );
            m_fbUser->setEmail( m_strCurrentUserEmail );
//            m_fbUser->setPassword( m_strCurrentUserPwd );
            m_fbUser->setAccess_token( m_strCurrentAccessToken );

            m_fbUserFeedPostList = OV_NEW(Domaine_App) OVData::FbUserfeedpostList();

            requestUserName( m_strCurrentUserId );
        }
    }

    void AppFacebookJob::startRequestsForCategoryFriendList()
    {
        if( m_strCurrentUserId.isNull() || m_strCurrentUserId.isEmpty() )
        {
            OVFB_WARNING( "current user ID null or empty, cannot start friendlist requests!" );
            setJobTerminated( 4, "friendlist" );
        }
        else
        {
            m_fbFriendList = OV_NEW(Domaine_App) OVData::FbUserFriendList();
            requestFriendsList( m_strCurrentUserId );
        }
    }

    void AppFacebookJob::startRequestsForCategoryFanPage()
    {
        if( m_strCurrentFanPageId.isNull() || m_strCurrentFanPageId.isEmpty() )
        {
            OVFB_WARNING( "current fanpage ID null or empty, cannot start fan page requests!" );
            setJobTerminated( 1, "fanpage" );
        }
        else
        {
            OVFB_MSG( "Starting requests for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

            if( !m_fbFanPage )
            {
                m_fbFanPage = OV_NEW(Domaine_App) OVData::FbFanpage();
            }
            m_fbFanPage->setPage_id( m_strCurrentFanPageId );

            m_fbFanpagePostList = OV_NEW(Domaine_App) OVData::FbFanpagePostList();

            requestFanPageInformation( m_strCurrentFanPageId );
        }
    }

    void AppFacebookJob::requestUserId()
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start user ID request!" );
            setJobTerminated( -1, "user_id" );
        }
        else
        {
            OVFB_MSG( "Starting request for user ID with email '%s' and access_token %s", m_strCurrentUserEmail.toUtf8().constData(), m_strCurrentAccessToken.toUtf8().constData() );

            if( !m_fbUser )
            {
                m_fbUser = OV_NEW(Domaine_App) OVData::FbUser();
            }
            m_fbUser->setEmail( m_strCurrentUserEmail );
//            m_fbUser->setPassword( m_strCurrentUserPwd );
            m_fbUser->setAccess_token( m_strCurrentAccessToken );

            m_strHandleFailedRequestTag = "user_id";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->retrieveUserId();
        }
    }

    void AppFacebookJob::requestUserName( QString _strUserId )
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start user name request!" );
            setJobTerminated( 0, "user_news_feed" );
        }
        else
        {
            m_strHandleFailedRequestTag = "user_name";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->retrieveUserName( _strUserId );
        }
    }

    void AppFacebookJob::requestUserPicture( QString _strUserId )
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start user picture request!" );
            setJobTerminated( 0, "user_news_feed" );
        }
        else
        {
            m_strHandleFailedRequestTag = "user_picture";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->retrieveUserPicture( _strUserId );
        }
    }

    void AppFacebookJob::requestUserFeedPostList( QString _strUserId )
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start user feed post list request!" );
            setJobTerminated( 0, "user_news_feed" );
        }
        else
        {
            m_strHandleFailedRequestTag = "user_feed_post_list";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->retrieveUserFeedPostList( _strUserId );
        }
    }

    void AppFacebookJob::requestUserFeedPostersName()
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start user feed posters name request!" );
            setJobTerminated( 0, "user_news_feed" );
        }
        else
        {
            m_strHandleFailedRequestTag = "user_feed_poster_name";

            m_iFriendNumber = 0;

            if( m_iNbUserFeedPosters != 0 )
            {
                if( !m_fbUserFeedPostList->posts().isEmpty() )
                {
                    QString strUserFeedPosterUserId = m_userFeedPosters.at( 0 );
                    if( !strUserFeedPosterUserId.isNull() && !strUserFeedPosterUserId.isEmpty() )
                    {
                        m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
                        m_facebookLib->retrieveUserFeedPosterName( strUserFeedPosterUserId );
                    }
                }
                else
                {
                    OVFB_WARNING( "user feed post list empty!" );
                    setJobTerminated( 0, "user_news_feed" );
                }
            }
            else
            {
                OVFB_WARNING( "user feed post list empty!" );
                setJobTerminated( 0, "user_news_feed" );
            }
        }
    }

    void AppFacebookJob::requestUserFeedPostersPicture()
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start user feed posters picture request!" );
            setJobTerminated( 0, "user_news_feed" );
        }
        else
        {
            m_strHandleFailedRequestTag = "user_feed_poster_picture";

            m_iFriendNumber = 0;

            if( m_iNbUserFeedPosters != 0 )
            {
                if( !m_fbUserFeedPostList->posts().isEmpty() )
                {
                    QString strUserFeedPosterUserId = m_userFeedPosters.at( 0 );
                    if( !strUserFeedPosterUserId.isNull() && !strUserFeedPosterUserId.isEmpty() )
                    {
                        m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
                        m_facebookLib->retrieveUserFeedPosterPicture( strUserFeedPosterUserId );
                    }
                }
                else
                {
                    OVFB_WARNING( "user feed post list empty!" );
                    setJobTerminated( 0, "user_news_feed" );
                }
            }
            else
            {
                OVFB_WARNING( "user feed post list empty!" );
                setJobTerminated( 0, "user_news_feed" );
            }
        }
    }

    void AppFacebookJob::requestFriendsList( QString _strUserId )
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start friendlist request!" );
            setJobTerminated( 4, "friendlist" );
        }
        else
        {
            m_strHandleFailedRequestTag = "friends";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->retrieveFriendsList( _strUserId );
        }
    }

    void AppFacebookJob::requestFriendsPicture()
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start friends picture request!" );
            setJobTerminated( 4, "friendlist" );
        }
        else
        {
            m_strHandleFailedRequestTag = "friends_picture";
            OVFB_MSG( "Request friends picture" );

            m_iFriendNumber = 0;
            int iNbFriends = m_fbFriendList->friends().size();

            if( !m_fbFriendList->friends().isEmpty() )
            {
                OVData::FbFriend * fbFriend = m_fbFriendList->friends().at( 0 );
                if( fbFriend )
                {
                    QString strFriendUserId = fbFriend->user_id();
                    if( !strFriendUserId.isNull() && !strFriendUserId.isEmpty() )
                    {
                        m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
                        m_facebookLib->retrieveFriendPicture( strFriendUserId );
                    }
                }
            }
            else
            {
                OVFB_WARNING( "friend list empty!" );
                setJobTerminated( 4, "friendlist" );
            }
        }
    }

    void AppFacebookJob::requestFriendsLikes()
    {
        m_bCurrentUserLikeSearch = false;

        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start friends likes request!" );
            setJobTerminated( 4, "friendlist" );
        }
        else
        {
            m_strHandleFailedRequestTag = "friends_likes";
            OVFB_MSG( "Request friends likes" );

            // add friends likes here. start with first friend, then add others in handle method.

            m_iFriendNumber = 0;
            int iNbFriends = m_fbFriendList->friends().size();

            if( !m_fbFriendList->friends().isEmpty() )
            {
                OVData::FbFriend * fbFriend = m_fbFriendList->friends().at( 0 );
                QString strFriendUserId = fbFriend->user_id();
                m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
                m_facebookLib->retrieveUserLikes( strFriendUserId );
            }
            else
            {
                OVFB_WARNING( "friend list empty!" );
                setJobTerminated( 4, "friendlist" );
            }
        }
    }

    void AppFacebookJob::requestUserLikes( QString _strUserId )
    {
        m_bCurrentUserLikeSearch = true;

        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start user likes request!" );
//            setJobTerminated( 4, "friendlist" );
        }
        else
        {
            m_strHandleFailedRequestTag = "user_likes";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->retrieveUserLikes( _strUserId );
        }
    }

    void AppFacebookJob::requestFanPageInformation( QString _strFanPageId )
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start fanpage information request!" );
            setJobTerminated( 1, "fanpage" );
        }
        else
        {
            m_strHandleFailedRequestTag = "fan_page_information";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->retrieveFanPageInformation( _strFanPageId );
        }
    }

    void AppFacebookJob::requestFanPagePicture( QString _strFanPageId )
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start fanpage picture request!" );
            setJobTerminated( 1, "fanpage" );
        }
        else
        {
            m_strHandleFailedRequestTag = "fan_page_picture";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->retrieveFanPagePicture( _strFanPageId );
        }
    }

    void AppFacebookJob::requestFanPagePostList( QString _strFanPageId )
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start fanpage post list request!" );
            setJobTerminated( 1, "fanpage" );
        }
        else
        {
            m_strHandleFailedRequestTag = "fan_page_post_list";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->retrieveFanPagePostList( _strFanPageId );
        }
    }

    void AppFacebookJob::requestFanPagePostersName()
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start fanpage posters name request!" );
            setJobTerminated( 1, "fanpage" );
        }
        else
        {
            m_strHandleFailedRequestTag = "fan_page_poster_name";

            m_iFriendNumber = 0;

            if( m_iNbFanPagePosters != 0 )
            {
                if( !m_fbFanpagePostList->posts().isEmpty() )
                {
                    QString strFanPagePosterUserId = m_fanPagePosters.at( 0 );
                    if( !strFanPagePosterUserId.isNull() && !strFanPagePosterUserId.isEmpty() )
                    {
                        m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
                        m_facebookLib->retrieveFanPagePosterName( strFanPagePosterUserId );
                    }
                }
                else
                {
                    OVFB_WARNING( "fan page post list empty!" );
                    setJobTerminated( 1, "fanpage" );
                }
            }
            else
            {
                OVFB_MSG( "fan page post list empty!" );
                setJobTerminated( 1, "fanpage" );
            }
        }
    }

    void AppFacebookJob::requestFanPagePostersPicture()
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start fanpage posters picture request!" );
            setJobTerminated( 1, "fanpage" );
        }
        else
        {
            m_strHandleFailedRequestTag = "fan_page_poster_picture";

            m_iFriendNumber = 0;

            if( m_iNbFanPagePosters != 0 )
            {
                if( !m_fbFanpagePostList->posts().isEmpty() )
                {
                    QString strFanPagePosterUserId = m_fanPagePosters.at( 0 );
                    if( !strFanPagePosterUserId.isNull() && !strFanPagePosterUserId.isEmpty() )
                    {
                        m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
                        m_facebookLib->retrieveFanPagePosterPicture( strFanPagePosterUserId );
                    }
                }
                else
                {
                    OVFB_WARNING( "fan page post list empty!" );
                    setJobTerminated( 1, "fanpage" );
                }
            }
            else
            {
                OVFB_WARNING( "fan page post list empty!" );
                setJobTerminated( 1, "fanpage" );
            }
        }
    }

    void AppFacebookJob::requestSearchFanPage( QString _strContent )
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start search fanpage request!" );
            if( m_strCurrentJob == "fbapi__content_likes" )
            {
                OVFB_MSG( "Sending message to UI - content title: %s; nb likes: %d", m_socialMessage.m_strTitle.toUtf8().constData(), m_socialMessage.m_iLikeNumber );
                emit sigSendMessageToUI( 0, QVariant().fromValue( m_socialMessage ) );
                setJobTerminated( -1, "fbapi__content_likes" );
            }
            else if( m_strCurrentJob == "like_content" )
            {
                setJobTerminated( -1, "like_content" );
            }
            else if( m_strCurrentJob == "unlike_content" )
            {
                setJobTerminated( -1, "unlike_content" );
            }
            else if( m_strCurrentJob == "share_message" )
            {
                setJobTerminated( -1, "share_message" );
            }
//            else if( m_strCurrentJob == "search_fanpage" )
            else
            {
                setJobTerminated( 3, "search_fanpage" );
            }
        }
        else
        {
            m_strHandleFailedRequestTag = "search";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->searchFanPage( _strContent );
        }
    }

    void AppFacebookJob::requestShareMessage( QString _strUserId, QString _strContent, QString _strMessage )
    {
        if( m_strCurrentAccessToken.isNull() || m_strCurrentAccessToken.isEmpty() )
        {
            OVFB_WARNING( "current access token null or empty, cannot start share message request!" );
            setJobTerminated( -1, "share_message" );
        }
        else
        {
            m_strHandleFailedRequestTag = "share";
            m_facebookLib->setCurrentAccessToken( m_strCurrentAccessToken );
            m_facebookLib->postMessage( _strUserId, _strContent, _strMessage );
        }
    }


    QDateTime AppFacebookJob::convertFbDateTimeToQDateTime( QString _strFbDateTime )
    {
        int iYear = _strFbDateTime.left( 4 ).toInt();

        _strFbDateTime.remove( 0, 5 );
        int iMonth = _strFbDateTime.left( 2 ).toInt();

        _strFbDateTime.remove( 0, 3 );
        int iDay = _strFbDateTime.left( 2 ).toInt();

        _strFbDateTime.remove( 0, 3 );
        int iHour = _strFbDateTime.left( 2 ).toInt();

        _strFbDateTime.remove( 0, 3 );
        int iMinute = _strFbDateTime.left( 2 ).toInt();

        _strFbDateTime.remove( 0, 3 );
        int iSecond = _strFbDateTime.left( 2 ).toInt();

        QDate fbDate( iYear, iMonth, iDay );
        QTime fbTime( iHour, iMinute, iSecond );

        QDateTime fbQDateTime( fbDate, fbTime );

        return fbQDateTime;
    }


// private slots

    void AppFacebookJob::slotHandleFailedRequest()
    {
        OVFB_IMPORTANT_MSG( "Failed request" );

        if( m_strHandleFailedRequestTag == "user_id" )
        {
            slotHandleUserIdRequest();
        }
        if( m_strHandleFailedRequestTag == "user_name" )
        {
            slotHandleUserNameRequest();
        }
        if( m_strHandleFailedRequestTag == "user_picture" )
        {
            slotHandleUserPictureRequest();
        }
        if( m_strHandleFailedRequestTag == "user_feed_post_list" )
        {
            slotHandleUserFeedPostListRequest();
        }
        if( m_strHandleFailedRequestTag == "user_feed_poster_name" )
        {
            slotHandleUserFeedPostersNameRequest();
        }
        if( m_strHandleFailedRequestTag == "user_feed_poster_picture" )
        {
            slotHandleUserFeedPostersPictureRequest();
        }
        if( m_strHandleFailedRequestTag == "friends" )
        {
            slotHandleFriendsListRequest();
        }
        if( m_strHandleFailedRequestTag == "friends_picture" )
        {
            slotHandleFriendsPictureRequest();
        }
        if( m_strHandleFailedRequestTag == "friends_likes" )
        {
            slotHandleUserLikesRequest();
        }
        if( m_strHandleFailedRequestTag == "user_likes" )
        {
            slotHandleUserLikesRequest();
        }
        if( m_strHandleFailedRequestTag == "fan_page_information" )
        {
            slotHandleFanPageInformationRequest();
        }
        if( m_strHandleFailedRequestTag == "fan_page_picture" )
        {
            slotHandleFanPagePictureRequest();
        }
        if( m_strHandleFailedRequestTag == "fan_page_post_list" )
        {
            slotHandleFanPagePostListRequest();
        }
        if( m_strHandleFailedRequestTag == "fan_page_poster_name" )
        {
            slotHandleFanPagePostersNameRequest();
        }
        if( m_strHandleFailedRequestTag == "fan_page_poster_picture" )
        {
            slotHandleFanPagePostersPictureRequest();
        }
        if( m_strHandleFailedRequestTag == "search" )
        {
            slotHandleSearchFanPageRequest();
        }
        if( m_strHandleFailedRequestTag == "share" )
        {
            slotHandleShareMessageRequest();
        }
    }

    void AppFacebookJob::slotHandleUserIdRequest()
    {
        OVFB_IMPORTANT_MSG( "Handling user ID request for user with access_token %s", m_strCurrentAccessToken.toUtf8().constData() );

        m_strCurrentUserId = m_facebookLib->getUserIdFound();
        OVFB_MSG("user ID found: %s", m_strCurrentUserId.toUtf8().constData() );

        m_fbUser->setUser_id( m_strCurrentUserId );

        if( !m_dbHandler->addUserToDB( m_fbUser ) )
        {
            OVFB_ERROR( "Error while adding user to database!" );
        }

        m_FBInterface->setCurrentUserId( m_strCurrentUserId );
        m_dbHandler->setCurrentUserId( m_strCurrentUserId );

        m_strHandleFailedRequestTag = "default";
        setJobTerminated( -1, "user_id" );
    }

    void AppFacebookJob::slotHandleUserNameRequest()
    {
        OVFB_IMPORTANT_MSG( "Handling user name request for user %s", m_strCurrentUserId.toUtf8().constData() );

        QString strUserName = m_facebookLib->getUserNameFound();
        OVFB_MSG("user name found: %s", strUserName.toUtf8().constData() );

        m_fbUser->setName( strUserName );

        m_strHandleFailedRequestTag = "default";
        requestUserPicture( m_strCurrentUserId );
    }

    void AppFacebookJob::slotHandleUserPictureRequest()
    {
        OVFB_IMPORTANT_MSG( "Handling user picture request for user %s", m_strCurrentUserId.toUtf8().constData() );

        QImage userPicture = m_facebookLib->getUserPictureFound();
        if( userPicture.isNull() )
        {
            OVFB_WARNING( "Error : picture from Facebook API is NULL! Cannot save picture" );
        }
        else
        {
            if( m_strPicturesFilepath.isNull() || m_strPicturesFilepath.isEmpty() )
            {
                OVFB_WARNING( "Error : pictures filepath null or empty! Cannot save picture" );
            }
            else
            {
                QString strUserPicturesFilepath = m_strPicturesFilepath + "/fb_users_pictures";
                if( !QDir( strUserPicturesFilepath ).exists() )
                {
                    OVFB_MSG( "directory %s does not exist, creating directory.", strUserPicturesFilepath.toUtf8().constData() );
                    if( !QDir().mkpath( strUserPicturesFilepath ) )
                    {
                        OVFB_ERROR( "Error while creating directory %s", strUserPicturesFilepath.toUtf8().constData() );
                    }
                }

                QString strPictureFilename = strUserPicturesFilepath + "/" + m_strCurrentUserId + ".jpg";
                QString strPictureFilenameForDB = "fb_users_pictures/" + m_strCurrentUserId + ".jpg";

                if( userPicture.save( strPictureFilename ) )
                {
                    OVFB_MSG( "Saving image from user %s: OK!", m_strCurrentUserId.toUtf8().constData() );
                    m_fbUser->setPicture( strPictureFilenameForDB );
                }
                else
                {
                    OVFB_ERROR( "Failed saving image from user %s !", m_strCurrentUserId.toUtf8().constData() );
                }
            }

            if( !m_dbHandler->addUserToDB( m_fbUser ) )
            {
                OVFB_ERROR( "Error while adding user to database!" );
            }
        }

        m_strHandleFailedRequestTag = "default";
        requestUserFeedPostList( m_strCurrentUserId );
    }

    void AppFacebookJob::slotHandleUserFeedPostListRequest()
    {
        OVFB_IMPORTANT_MSG( "Handling user feed post list request for user %s", m_strCurrentUserId.toUtf8().constData() );
        QMap< int, QMap<QString, QString> > userFeedPostList;
        userFeedPostList = m_facebookLib->getUserFeedPostListFound();

        m_userFeedPosters.clear();
        if( userFeedPostList.size() != 0 )
        {
            QList<OVData::FbUserfeedpost* > fbUserFeedPostQList;
            QMapIterator< int, QMap<QString, QString> > i( userFeedPostList );
            while( i.hasNext() )
            {
                i.next();
//                OVFB_MSG( "%d - user id: %s", i.key(), i.value()["user_id"].toUtf8().constData() );
//                OVFB_MSG( "%d - time: %s", i.key(), i.value()["time"].toUtf8().constData() );
//                OVFB_MSG( "%d - message: %s", i.key(), i.value()["message"].toUtf8().constData() );
//                if( !i.value()["msg_name"].isNull() && !i.value()["msg_name"].isEmpty() )
//                {
//                    OVFB_MSG( "%d - message name: %s", i.key(), i.value()["msg_name"].toUtf8().constData() );
//                }
//                OVFB_MSG( "%d - post id: %s", i.key(), i.value()["post_id"].toUtf8().constData() );

                QDateTime userFeedPostTime = convertFbDateTimeToQDateTime( i.value()["time"] );
//                OVFB_MSG( "%d - converted time: %s", i.key(), userFeedPostTime.toString().toUtf8().constData() );

                OVData::FbUserfeedpost* fbUserFeedPost = OV_NEW(Domaine_App) OVData::FbUserfeedpost();
                fbUserFeedPost->setUser_id( i.value()["user_id"] );
                fbUserFeedPost->setTime( userFeedPostTime );
                if( !i.value()["msg_name"].isNull() && !i.value()["msg_name"].isEmpty() )
                {
                    fbUserFeedPost->setMessage( i.value()["msg_name"] + ": " + i.value()["message"] );
//                    fbUserFeedPost->setMessage( i.value()["message"] + "|||" + i.value()["msg_name"] );
                }
                else
                {
                    fbUserFeedPost->setMessage( i.value()["message"] );
                }
                fbUserFeedPost->setPost_id( i.value()["post_id"] );

                OVData::FbFriend* fbFriend = OV_NEW(Domaine_App) OVData::FbFriend();
                fbFriend->setUser_id( i.value()["user_id"] );

                if( !m_userFeedPosters.contains( i.value()["user_id"] ) )
                {
                    m_userFeedPosters.append( i.value()["user_id"] );
                }

                if( !m_dbHandler->addUserFeedPostToDB( fbUserFeedPost ) )
                {
                    OVFB_ERROR( "Error while adding feed post to database!" );
                }

                if( !m_dbHandler->addFriendToDB( fbFriend ) )
                {
                    OVFB_ERROR( "Error while adding friend to database!" );
                }

                fbUserFeedPostQList.append( fbUserFeedPost );
                OV_DELETE( fbFriend );
            }

            m_iNbUserFeedPosters = m_userFeedPosters.size();

            m_fbUserFeedPostList->setPosts( fbUserFeedPostQList );

            if( m_fbUser )
            {
                m_fbUserFeedPostList->setUser( m_fbUser );
            }
            else
            {
                OVFB_MSG( "no fbuser" );
            }
            if( !m_dbHandler->addUserFeedPostListToDB( m_fbUserFeedPostList ) )
            {
                OVFB_ERROR( "Error while adding feed post list to database!" );
            }

            m_fbUser->setUserfeedpostList( m_fbUserFeedPostList );
            if( !m_dbHandler->addUserToDB( m_fbUser ) )
            {
                OVFB_ERROR( "Error while adding user to database!" );
            }
            m_strHandleFailedRequestTag = "default";
            requestUserFeedPostersName();
        }
        else
        {
            OVFB_MSG( "No user feed post list found with API!" );
            m_iNbUserFeedPosters = 0;
            m_strHandleFailedRequestTag = "default";
            setJobTerminated( 0, "user_news_feed" );
        }
    }

    void AppFacebookJob::slotHandleUserFeedPostersNameRequest()
    {
        QString strUserFeedPosterUserId = m_userFeedPosters.at( m_iFriendNumber );

        OVFB_IMPORTANT_MSG( "Handling name request for user feed poster %s", strUserFeedPosterUserId.toUtf8().constData() );

        if( !strUserFeedPosterUserId.isNull() || !strUserFeedPosterUserId.isEmpty() )
        {
            QString strUserFeedPosterName = m_facebookLib->getUserFeedPosterNameFound();
            OVFB_MSG("user feed poster name found: %s", strUserFeedPosterName.toUtf8().constData() );

            OVData::FbFriend* fbUserFeedPoster = OV_NEW(Domaine_App) OVData::FbFriend();
            fbUserFeedPoster->setUser_id( strUserFeedPosterUserId );
            fbUserFeedPoster->setName( strUserFeedPosterName );

            if( !m_dbHandler->addFriendToDB( fbUserFeedPoster ) )
            {
                OVFB_ERROR( "Error while adding user feed poster to database!" );
            }
            OV_DELETE( fbUserFeedPoster );
        }

        m_iFriendNumber++;

        if( m_iFriendNumber < m_iNbUserFeedPosters )
        {
            if( !m_fbUserFeedPostList->posts().isEmpty() )
            {
                QString strUserFeedPosterUserId = m_userFeedPosters.at( m_iFriendNumber );

                if( !strUserFeedPosterUserId.isNull() && !strUserFeedPosterUserId.isEmpty() )
                {
                    m_facebookLib->retrieveUserFeedPosterName( strUserFeedPosterUserId );
                }
            }
            else
            {
                OVFB_MSG( "user feed post list empty!" );
                m_strHandleFailedRequestTag = "default";
                setJobTerminated( 0, "user_news_feed" );
            }
        }
        else
        {
            OVFB_MSG( "User feed Posters name: Done!" );
            m_strHandleFailedRequestTag = "default";
            requestUserFeedPostersPicture();
        }
    }

    void AppFacebookJob::slotHandleUserFeedPostersPictureRequest()
    {
        QString strUserFeedPosterUserId = m_userFeedPosters.at( m_iFriendNumber );

        OVFB_IMPORTANT_MSG( "Handling picture request for user feed poster %s", strUserFeedPosterUserId.toUtf8().constData() );

        OVData::FbFriend* fbUserFeedPoster = m_dbHandler->getFriendFromDB( strUserFeedPosterUserId );
        if( !fbUserFeedPoster )
        {
            fbUserFeedPoster = OV_NEW(Domaine_App) OVData::FbFriend();
            fbUserFeedPoster->setUser_id( strUserFeedPosterUserId );
        }

        QImage userFeedPosterPicture = m_facebookLib->getUserFeedPosterPictureFound();
        if( userFeedPosterPicture.isNull() )
        {
            OVFB_WARNING( "Error : user feed poster picture from Facebook API is NULL! Cannot save picture" );
        }
        else
        {
            if( m_strPicturesFilepath.isNull() || m_strPicturesFilepath.isEmpty() )
            {
                OVFB_WARNING( "Error : pictures filepath null or empty! Cannot save picture" );
            }
            else
            {
                QString strUserFeedPosterPicturesFilepath = m_strPicturesFilepath + "/fb_users_pictures";
                if( !QDir( strUserFeedPosterPicturesFilepath ).exists() )
                {
                    OVFB_MSG( "directory %s does not exist, creating directory.", strUserFeedPosterPicturesFilepath.toUtf8().constData() );
                    if( !QDir().mkpath( strUserFeedPosterPicturesFilepath ) )
                    {
                        OVFB_ERROR( "Error while creating directory %s", strUserFeedPosterPicturesFilepath.toUtf8().constData() );
                    }
                }

                QString strPictureFilename = strUserFeedPosterPicturesFilepath + "/" + strUserFeedPosterUserId + ".jpg";
                QString strPictureFilenameForDB = "fb_users_pictures/" + strUserFeedPosterUserId + ".jpg";

                if( userFeedPosterPicture.save( strPictureFilename ) )
                {
                    OVFB_MSG( "Saving image from user feed poster %s: OK!", strUserFeedPosterUserId.toUtf8().constData() );
                    fbUserFeedPoster->setPicture( strPictureFilenameForDB );
                }
                else
                {
                    OVFB_ERROR( "Failed saving image from user feed poster %s !", strUserFeedPosterUserId.toUtf8().constData() );
                }
            }

            if( !m_dbHandler->addFriendToDB( fbUserFeedPoster ) )
            {
                OVFB_ERROR( "Error while adding user feed poster to database!" );
            }
        }

        m_iFriendNumber++;

        if( m_iFriendNumber < m_iNbUserFeedPosters )
        {
            if( !m_fbUserFeedPostList->posts().isEmpty() )
            {
                QString strUserFeedPosterUserId = m_userFeedPosters.at( m_iFriendNumber );
                if( !strUserFeedPosterUserId.isNull() && !strUserFeedPosterUserId.isEmpty() )
                {
                    m_facebookLib->retrieveUserFeedPosterPicture( strUserFeedPosterUserId );
                }
            }
            else
            {
                OVFB_MSG( "user feed post list empty!" );
                m_strHandleFailedRequestTag = "default";
                setJobTerminated( 0, "user_news_feed" );
            }
        }
        else
        {
            OVFB_MSG( "User feed Posters picture: Done!" );
            m_strHandleFailedRequestTag = "default";
//            setJobTerminated( 0, "user_news_feed" );
            requestUserLikes( m_strCurrentUserId );
        }
    }

    void AppFacebookJob::slotHandleFriendsListRequest()
    {
        OVFB_IMPORTANT_MSG( "Handling friends list request for user %s", m_strCurrentUserId.toUtf8().constData() );
        QMap< int, QMap<QString, QString> > friendsList;
        friendsList = m_facebookLib->getFriendsList();

        m_iNbFriends = friendsList.size();

        if( m_iNbFriends != 0 )
        {
            OVFB_MSG( "Number of friends found for user %s: %d", m_strCurrentUserId.toUtf8().constData(), m_iNbFriends );

            QList<OVData::FbFriend *> fbFriendQList;
            QMapIterator< int, QMap<QString, QString> > i( friendsList );
            while( i.hasNext() )
            {
                i.next();
//                OVFB_MSG( "friend %d - ID: %s - name: %s", i.key(), i.value()["id"].toUtf8().constData(), i.value()["name"].toUtf8().constData() );

                OVData::FbFriend* fbFriend = OV_NEW(Domaine_App) OVData::FbFriend();
                fbFriend->setUser_id( i.value()["id"] );
                fbFriend->setName( i.value()["name"] );

                if( !m_dbHandler->addFriendToDB( fbFriend ) )
                {
                    OVFB_ERROR( "Error while adding friend to database!" );
                }

                fbFriendQList.append( fbFriend );
            }
            m_fbFriendList->setFriends( fbFriendQList );
            if( m_fbUser )
            {
                m_fbFriendList->setUser( m_fbUser );
            }
            else
            {
                OVFB_MSG( "no fbuser" );
            }
            if( !m_dbHandler->addUserFriendListToDB( m_fbFriendList ) )
            {
                OVFB_ERROR( "Error while adding friend list to database!" );
            }

            m_fbUser->setFriendList( m_fbFriendList );
            if( !m_dbHandler->addUserToDB( m_fbUser ) )
            {
                OVFB_ERROR( "Error while adding user to database!" );
            }
            m_strHandleFailedRequestTag = "default";
            requestFriendsPicture();
        }
        else
        {
            OVFB_MSG( "No friendlist found with API!" );
            m_strHandleFailedRequestTag = "default";
            setJobTerminated( 4, "friendlist" );
        }
    }

    void AppFacebookJob::slotHandleFriendsPictureRequest()
    {
        if( m_iNbFriends > 0 )
        {
            OVData::FbFriend * fbFriend = m_fbFriendList->friends().at( m_iFriendNumber );
            QString strFriendUserId = fbFriend->user_id();
//            OVFB_MSG( "(%d) id of friend: %s", m_iFriendNumber, strFriendUserId.toUtf8().constData() );

            OVFB_IMPORTANT_MSG( "Handling picture request for friend %s", strFriendUserId.toUtf8().constData() );

            QImage friendPicture = m_facebookLib->getFriendPictureFound();
            if( friendPicture.isNull() )
            {
                OVFB_WARNING( "Error : friend picture from Facebook API is NULL! Cannot save picture" );
            }
            else
            {
                if( m_strPicturesFilepath.isNull() || m_strPicturesFilepath.isEmpty() )
                {
                    OVFB_WARNING( "Error : pictures filepath null or empty! Cannot save picture" );
                }
                else
                {
                    QString strFriendPicturesFilepath = m_strPicturesFilepath + "/fb_users_pictures";
                    if( !QDir( strFriendPicturesFilepath ).exists() )
                    {
                        OVFB_MSG( "directory %s does not exist, creating directory.", strFriendPicturesFilepath.toUtf8().constData() );
                        if( !QDir().mkpath( strFriendPicturesFilepath ) )
                        {
                            OVFB_ERROR( "Error while creating directory %s", strFriendPicturesFilepath.toUtf8().constData() );
                        }
                    }

                    QString strPictureFilename = strFriendPicturesFilepath + "/" + strFriendUserId + ".jpg";
                    QString strPictureFilenameForDB = "fb_users_pictures/" + strFriendUserId + ".jpg";

                    if( friendPicture.save( strPictureFilename ) )
                    {
                        OVFB_MSG( "Saving image from friend %s: OK!", strFriendUserId.toUtf8().constData() );
                        fbFriend->setPicture( strPictureFilenameForDB );
                    }
                    else
                    {
                        OVFB_ERROR( "Failed saving image from friend %s !", strFriendUserId.toUtf8().constData() );
                    }
                }

                if( !m_dbHandler->addFriendToDB( fbFriend ) )
                {
                    OVFB_ERROR( "Error while adding friend to database!" );
                }
            }

            m_iFriendNumber++;

            if( m_iFriendNumber < m_iNbFriends )
            {
                if( !m_fbFriendList->friends().isEmpty() )
                {
                    OVData::FbFriend * fbFriend = m_fbFriendList->friends().at( m_iFriendNumber );
                    strFriendUserId = fbFriend->user_id();
                    m_facebookLib->retrieveFriendPicture( strFriendUserId );
                }
                else
                {
                    OVFB_MSG( "friend list empty!" );
                    m_strHandleFailedRequestTag = "default";
                    setJobTerminated( 4, "friendlist" );
                }
            }
            else
            {
                OVFB_MSG( "Friends picture: Done!" );
                m_strHandleFailedRequestTag = "default";
                requestFriendsLikes();
            }
        }
        else
        {
            OVFB_MSG( "friend list empty!" );
            m_strHandleFailedRequestTag = "default";
            setJobTerminated( 4, "friendlist" );
        }
    }

    void AppFacebookJob::slotHandleUserLikesRequest()
    {
        QStringList userLikesFound = m_facebookLib->getUserLikesFound();

        int iNbUserLikesFound = userLikesFound.size();

        if( m_bCurrentUserLikeSearch )
        {
            // Handle user likes request for current user
            OVFB_IMPORTANT_MSG( "Handling user likes request for current user %s", m_strCurrentUserId.toUtf8().constData() );

            for( int i = 0; i < iNbUserLikesFound; i++ )
            {
                OVData::FbFanpage* fbFanPage = OV_NEW(Domaine_App) OVData::FbFanpage();
                fbFanPage->setPage_id( userLikesFound.at( i ) );

                if( !m_dbHandler->addFanPageToDB( fbFanPage ) )
                {
                    OVFB_ERROR( "Error while adding fan page to database!" );
                }

                m_dbHandler->addUserToFanPageUserListToDB( fbFanPage, m_fbUser );
                m_fbFanPageQList.append( fbFanPage );
            }

            OVFB_MSG( "User likes: Done!" );
            m_strHandleFailedRequestTag = "default";
            setJobTerminated( 0, "user_news_feed" );
        }
        else
        {
            // Handle user likes request for friends
            OVData::FbFriend* fbFriend = m_fbFriendList->friends().at( m_iFriendNumber );

            QString strFriendId = fbFriend->user_id();

            OVFB_IMPORTANT_MSG( "Handling user likes request for friend %s", strFriendId.toUtf8().constData() );

            for( int i = 0; i < iNbUserLikesFound; i++ )
            {
                OVData::FbFanpage* fbFanPage = OV_NEW(Domaine_App) OVData::FbFanpage();
                fbFanPage->setPage_id( userLikesFound.at( i ) );

                if( !m_dbHandler->addFanPageToDB( fbFanPage ) )
                {
                    OVFB_ERROR( "Error while adding fan page to database!" );
                }

                m_dbHandler->addFriendToFanPageFriendListToDB( fbFanPage, fbFriend );

                m_fbFanPageQList.append( fbFanPage );
            }

            m_iFriendNumber++;

            if( m_iFriendNumber < m_iNbFriends )
            {
                if( !m_fbFriendList->friends().isEmpty() )
                {
                    fbFriend = m_fbFriendList->friends().at( m_iFriendNumber );
                    QString strFriendUserId = fbFriend->user_id();
                    m_facebookLib->retrieveUserLikes( strFriendUserId );
                }
                else
                {
                    OVFB_MSG( "friend list empty!" );
                    m_strHandleFailedRequestTag = "default";
                    setJobTerminated( 4, "friendlist" );
                }
            }
            else
            {
                OVFB_MSG( "Friends likes: Done!" );
                m_strHandleFailedRequestTag = "default";
                setJobTerminated( 4, "friendlist" );
            }
            //TODO: retrieve info of added fanpages?
        }
    }

    void AppFacebookJob::slotHandleUserLikesFailed()
    {
        OVFB_MSG( "Retry Request friends likes" );

        m_iFriendNumber = 0;
        int iNbFriends = m_fbFriendList->friends().size();

        if( !m_fbFriendList->friends().isEmpty() )
        {
            OVData::FbFriend * fbFriend = m_fbFriendList->friends().at( 0 );

            QString strFriendUserId = fbFriend->user_id();
            m_facebookLib->retrieveUserLikes( strFriendUserId );
        }
        else
        {
            OVFB_WARNING( "friend list empty!" );
        }
    }

    void AppFacebookJob::slotHandleFanPageInformationRequest()
    {
        OVFB_IMPORTANT_MSG( "Handling fan page information request for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );
        QMap<QString, QString> fanPageInformation;
        fanPageInformation = m_facebookLib->getFanPageInformationFound();

        if( !fanPageInformation["id"].isNull() && !fanPageInformation["id"].isEmpty() )
        {
            OVFB_MSG( "ID of fanpage found: %s", fanPageInformation["id"].toUtf8().constData() );
            m_fbFanPage->setPage_id( fanPageInformation["id"] );
        }
        if( !fanPageInformation["name"].isNull() && !fanPageInformation["name"].isEmpty() )
        {
            OVFB_MSG( "name: %s", fanPageInformation["name"].toUtf8().constData() );
            m_fbFanPage->setName( fanPageInformation["name"] );
        }
//            if( !fanPageInformation["picture"].isNull() && fanPageInformation["picture"].isEmpty() )
//            {
//                m_fbFanPage->setPicture( fanPageInformation["picture"] );
//            }
        if( !fanPageInformation["link"].isNull() && !fanPageInformation["link"].isEmpty() )
        {
            m_fbFanPage->setLink( fanPageInformation["link"] );
        }
        if( !fanPageInformation["category"].isNull() && !fanPageInformation["category"].isEmpty() )
        {
            m_fbFanPage->setCategory( fanPageInformation["category"] );
        }
        if( !fanPageInformation["starring"].isNull() && !fanPageInformation["starring"].isEmpty() )
        {
            m_fbFanPage->setStarring( fanPageInformation["starring"] );
        }
        if( !fanPageInformation["directed_by"].isNull() && !fanPageInformation["directed_by"].isEmpty() )
        {
            m_fbFanPage->setDirected_by( fanPageInformation["directed_by"] );
        }
        if( !fanPageInformation["plot_outline"].isNull() && !fanPageInformation["plot_outline"].isEmpty() )
        {
            m_fbFanPage->setPlot_outline( fanPageInformation["plot_outline"] );
        }
        if( !fanPageInformation["likes"].isNull() && !fanPageInformation["likes"].isEmpty() )
        {
            m_fbFanPage->setFan_count( fanPageInformation["likes"] );
        }

        if( !m_dbHandler->addFanPageToDB( m_fbFanPage ) )
        {
            OVFB_ERROR( "Error while adding fan page to database!" );
        }

        m_strHandleFailedRequestTag = "default";
        if( m_strCurrentJob == "share_message" )
        {
            requestShareMessage( m_strCurrentUserId, fanPageInformation["link"], m_strMessageToShare );
        }
        else
        {
            m_strHandleFailedRequestTag = "default";
            AppPluginJob::sendMessageToInterface( 1 );
            requestFanPagePicture( m_strCurrentFanPageId );
        }
    }

    void AppFacebookJob::slotHandleFanPagePictureRequest()
    {
        OVFB_IMPORTANT_MSG( "Handling fan page picture request for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );
        QImage fanPagePicture = m_facebookLib->getFanPagePictureFound();

        if( m_strPicturesFilepath.isNull() || m_strPicturesFilepath.isEmpty() )
        {
            OVFB_WARNING( "Error : pictures filepath null or empty!" );
        }
        else
        {
            QString strFanPagePicturesFilepath = m_strPicturesFilepath + "/fb_fanpages_pictures";
            if( !QDir( strFanPagePicturesFilepath ).exists() )
            {
                OVFB_MSG("directory %s does not exist, creating directory.", strFanPagePicturesFilepath.toUtf8().constData() );
                if( !QDir().mkpath( strFanPagePicturesFilepath ) )
                {
                    OVFB_ERROR("Error while creating directory %s", strFanPagePicturesFilepath.toUtf8().constData() );
                }
            }

            QString strPictureFilename = strFanPagePicturesFilepath + "/" + m_strCurrentFanPageId + ".jpg";
            QString strPictureFilenameForDB = "fb_fanpages_pictures/" + m_strCurrentFanPageId + ".jpg";

            if( fanPagePicture.save( strPictureFilename ) )
            {
                OVFB_MSG("Saving image from fan page %s: OK!", m_strCurrentFanPageId.toUtf8().constData() );
                m_fbFanPage->setPicture( strPictureFilenameForDB );
                if( !m_dbHandler->addFanPageToDB( m_fbFanPage ) )
                {
                    OVFB_ERROR( "Error while adding fan page to database!" );
                }
            }
            else
            {
                OVFB_ERROR("Failed saving image from fan page %s !", m_strCurrentFanPageId.toUtf8().constData() );
            }
        }

        if( !m_dbHandler->addFanPageToDB( m_fbFanPage ) )
        {
            OVFB_ERROR( "Error while adding fan page to database!" );
        }

        m_strHandleFailedRequestTag = "default";
        AppPluginJob::sendMessageToInterface( 1 );
        requestFanPagePostList( m_strCurrentFanPageId );
    }

    void AppFacebookJob::slotHandleFanPagePostListRequest()
    {
        OVFB_IMPORTANT_MSG( "Handling fan page posts request for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );
        QMap< int, QMap<QString, QString> > FanPagePostList;
        FanPagePostList = m_facebookLib->getFanPagePostListFound();

        m_fanPagePosters.clear();
        if( FanPagePostList.size() != 0 )
        {
            QList<OVData::FbFanpagePost *> fanpagePostQList;
            QMapIterator< int, QMap<QString, QString> > i( FanPagePostList );
            while( i.hasNext() )
            {
                i.next();
//                OVFB_MSG( "%d - user id %s", i.key(), i.value()["user_id"].toUtf8().constData() );
////                OVFB_MSG( "%d - time %s", i.key(), i.value()["time"].toUtf8().constData() );
//                OVFB_MSG( "%d - message %s", i.key(), i.value()["message"].toUtf8().constData() );
//                OVFB_MSG( "%d - post id %s", i.key(), i.value()["post_id"].toUtf8().constData() );

                QDateTime fanPagePostTime = convertFbDateTimeToQDateTime( i.value()["time"] );
//                OVFB_MSG( "%d - converted time %s", i.key(), fanPagePostTime.toString().toUtf8().constData() );

                OVData::FbFanpagePost* fbFanPagePost = OV_NEW(Domaine_App) OVData::FbFanpagePost();
                fbFanPagePost->setUser_id( i.value()["user_id"] );
                fbFanPagePost->setTime( fanPagePostTime );
                fbFanPagePost->setMessage( i.value()["message"] );
                fbFanPagePost->setPost_id( i.value()["post_id"] );

                if( !m_fanPagePosters.contains( i.value()["user_id"] ) )
                {
                    m_fanPagePosters.append( i.value()["user_id"] );
                }

                if( !m_dbHandler->addFanPagePostToDB( fbFanPagePost ) )
                {
                    OVFB_ERROR( "Error while adding fan page post to database!" );
                }

                fanpagePostQList.append( fbFanPagePost );
            }

            m_iNbFanPagePosters = m_fanPagePosters.size();

            m_fbFanpagePostList->setPosts( fanpagePostQList );
            if( m_fbFanPage )
            {
                m_fbFanpagePostList->setFanpage( m_fbFanPage );
            }
            else
            {
                OVFB_MSG( "no fbfanpage" );
            }
            if( !m_dbHandler->addFanPagePostListToDB( m_fbFanpagePostList ) )
            {
                OVFB_ERROR( "Error while adding fan page post list to database!" );
            }

            m_fbFanPage->setPostList( m_fbFanpagePostList );
            if( !m_dbHandler->addFanPageToDB( m_fbFanPage ) )
            {
                OVFB_ERROR( "Error while adding fan page to database!" );
            }
            m_strHandleFailedRequestTag = "default";
            requestFanPagePostersName();
        }
        else
        {
            OVFB_MSG( "No fan page post list found with API!" );
            m_iNbFanPagePosters = 0;
            m_strHandleFailedRequestTag = "default";
            setJobTerminated( 1, "fanpage" );
        }
    }

    void AppFacebookJob::slotHandleFanPagePostersNameRequest()
    {
        QString strFanPagePosterUserId = m_fanPagePosters.at( m_iFriendNumber );

        OVFB_IMPORTANT_MSG( "Handling name request for fan page poster %s", strFanPagePosterUserId.toUtf8().constData() );

        if( !strFanPagePosterUserId.isNull() || !strFanPagePosterUserId.isEmpty() )
        {
            QString strFanPagePosterName = m_facebookLib->getFanPagePosterNameFound();
            OVFB_MSG("fan page poster name found: %s", strFanPagePosterName.toUtf8().constData() );

            OVData::FbFriend* fbFanPagePoster = OV_NEW(Domaine_App) OVData::FbFriend();
            fbFanPagePoster->setUser_id( strFanPagePosterUserId );
            fbFanPagePoster->setName( strFanPagePosterName );

            if( !m_dbHandler->addFriendToDB( fbFanPagePoster ) )
            {
                OVFB_ERROR( "Error while adding fan page poster to database!" );
            }
        }

        m_iFriendNumber++;

        if( m_iFriendNumber < m_iNbFanPagePosters )
        {
            if( !m_fbFanpagePostList->posts().isEmpty() )
            {
                QString strFanPagePosterUserId = m_fanPagePosters.at( m_iFriendNumber );

                if( !strFanPagePosterUserId.isNull() && !strFanPagePosterUserId.isEmpty() )
                {
                    OVFB_MSG( "(%d) id of fan page poster: %s", m_iFriendNumber, strFanPagePosterUserId.toUtf8().constData() );
                    m_facebookLib->retrieveFanPagePosterName( strFanPagePosterUserId );;
                }
            }
            else
            {
                OVFB_MSG( "fan page post list empty!" );
                m_strHandleFailedRequestTag = "default";
                setJobTerminated( 1, "fanpage" );
            }
        }
        else
        {
            OVFB_MSG( "fan page Posters name: Done!" );
            m_strHandleFailedRequestTag = "default";
            requestFanPagePostersPicture();
        }
    }

    void AppFacebookJob::slotHandleFanPagePostersPictureRequest()
    {
        QString strFanPagePosterUserId = m_fanPagePosters.at( m_iFriendNumber );

        OVFB_IMPORTANT_MSG( "Handling picture request for fan page poster %s", strFanPagePosterUserId.toUtf8().constData() );

        OVData::FbFriend* fbFanPagePoster = m_dbHandler->getFriendFromDB( strFanPagePosterUserId );
        if( !fbFanPagePoster )
        {
            fbFanPagePoster = OV_NEW(Domaine_App) OVData::FbFriend();
            fbFanPagePoster->setUser_id( strFanPagePosterUserId );
        }


        QImage fanPagePosterPicture = m_facebookLib->getFanPagePosterPictureFound();
        if( fanPagePosterPicture.isNull() )
        {
            OVFB_WARNING( "Error : fan page poster picture from Facebook API is NULL! Cannot save picture" );
        }
        else
        {
            if( m_strPicturesFilepath.isNull() || m_strPicturesFilepath.isEmpty() )
            {
                OVFB_WARNING( "Error : pictures filepath null or empty! Cannot save picture" );
            }
            else
            {
                QString strFanPagePosterPicturesFilepath = m_strPicturesFilepath + "/fb_users_pictures";
                if( !QDir( strFanPagePosterPicturesFilepath ).exists() )
                {
                    OVFB_MSG( "directory %s does not exist, creating directory.", strFanPagePosterPicturesFilepath.toUtf8().constData() );
                    if( !QDir().mkpath( strFanPagePosterPicturesFilepath ) )
                    {
                        OVFB_ERROR( "Error while creating directory %s", strFanPagePosterPicturesFilepath.toUtf8().constData() );
                    }
                }

                QString strPictureFilename = strFanPagePosterPicturesFilepath + "/" + strFanPagePosterUserId + ".jpg";
                QString strPictureFilenameForDB = "fb_users_pictures/" + strFanPagePosterUserId + ".jpg";

                if( fanPagePosterPicture.save( strPictureFilename ) )
                {
                    OVFB_MSG( "Saving image from fan page poster %s: OK!", strFanPagePosterUserId.toUtf8().constData() );
                    fbFanPagePoster->setPicture( strPictureFilenameForDB );
                }
                else
                {
                    OVFB_ERROR( "Failed saving image from fan page poster %s !", strFanPagePosterUserId.toUtf8().constData() );
                }
            }

            if( !m_dbHandler->addFriendToDB( fbFanPagePoster ) )
            {
                OVFB_ERROR( "Error while adding fan page poster to database!" );
            }
        }

        m_iFriendNumber++;

        if( m_iFriendNumber < m_iNbFanPagePosters )
        {
            if( !m_fbFanpagePostList->posts().isEmpty() )
            {
                QString strFanPagePosterUserId = m_fanPagePosters.at( m_iFriendNumber );
                if( !strFanPagePosterUserId.isNull() && !strFanPagePosterUserId.isEmpty() )
                {
                    m_facebookLib->retrieveFanPagePosterPicture( strFanPagePosterUserId );
                }
            }
            else
            {
                OVFB_MSG( "fan page post list empty!" );
                m_strHandleFailedRequestTag = "default";
                setJobTerminated( 1, "fanpage" );
            }
        }
        else
        {
            OVFB_MSG( "fan page Posters picture: Done!" );
            m_strHandleFailedRequestTag = "default";
            setJobTerminated( 1, "fanpage" );
        }
    }

    void AppFacebookJob::slotHandleSearchFanPageRequest()
    {
        OVFB_IMPORTANT_MSG( "Handling fan page search request..." );
        m_fanPagesFound = m_facebookLib->getFanPagesIdsFound();

        m_fanPagesMap.clear();

        OVData::FbContent* fbContent = OV_NEW(Domaine_App) OVData::FbContent();
        fbContent->setContent_id( m_strContentId );
        fbContent->setType( m_strContentType );
        fbContent->setTitle( m_strContentTitle );

        if( !m_fanPagesFound.isEmpty() )
        {
            m_iNbFanPagesFound = m_fanPagesFound.size();

            for( int i = 0; i < m_iNbFanPagesFound; i++ )
            {
                OVFB_MSG( "ID of fan page found n°%d: %s", i, m_fanPagesFound.at( i ).toUtf8().constData() );
                OVData::FbFanpage* fbFanpage = OV_NEW(Domaine_App) OVData::FbFanpage();
                fbFanpage->setPage_id( m_fanPagesFound.at( i ) );

                if( !m_dbHandler->addFanPageToDB( fbFanpage ) )
                {
                    OVFB_ERROR( "Error while adding fan page to database!" );
                }

                fbContent->addFanpage( fbFanpage, false );
            }
            if( !m_dbHandler->addContentToDB( fbContent ) )
            {
                OVFB_ERROR( "Error while adding content to database!" );
            }


            if( m_strCurrentJob == "fbapi__content_likes" )
            {
                OVFB_MSG( "Specific actions for job: fbapi__content_likes" );
                checkIfCurrentUserLikesContent( m_strContentId );
                getLikesForContent();

                setJobTerminated( -1, "fbapi__content_likes" );
            }

            if( m_strCurrentJob == "like_content" )
            {
                OVFB_MSG( "Specific actions for job: like_content" );

                m_strCurrentFanPageId = m_fanPagesFound.at( 0 );
                QStringList fanPagesToLike;
                fanPagesToLike.append( m_strCurrentFanPageId );

                setJobTerminated( -1, "like_content" );

                OVFB_MSG( "Will make like for fanpage %s", m_strCurrentFanPageId.toUtf8().constData() );
                m_FBInterface->setCurrentUserEmail( m_strCurrentUserEmail );
                m_FBInterface->setCurrentUserPwd( m_strCurrentUserPwd );
                m_FBInterface->setFanPagesToLikeOrUnlike( fanPagesToLike );
                sendMessageToInterface( 5 );
            }

            if( m_strCurrentJob == "unlike_content" )
            {
                OVFB_MSG( "Specific actions for job: unlike_content" );

                m_fanPagesToUnlike.clear();

                for( int i = 0; i < m_iNbFanPagesFound; i++ )
                {
                    if( m_dbHandler->checkFanPageLike( m_strCurrentUserId, m_fanPagesFound.at( i ) ) )
                    {
                        m_fanPagesToUnlike.append( m_fanPagesFound.at( i ) );
                    }
                }

                setJobTerminated( -1, "unlike_content" );

                OVFB_MSG( "Will make unlike for fanpages" );
                m_FBInterface->setCurrentUserEmail( m_strCurrentUserEmail );
                m_FBInterface->setCurrentUserPwd( m_strCurrentUserPwd );
                m_FBInterface->setFanPagesToLikeOrUnlike( m_fanPagesToUnlike );
                sendMessageToInterface( 6 );
            }

            else if( m_strCurrentJob == "search_fanpage" )
            {
                OVFB_MSG( "Specific actions for job: search_fanpage" );


                // Get likes for this set of fan pages
                QString strFanPageId;
                QStringList friendsIds;
                QStringList pics;
                int iNbPicsToSend = 10;  // we send only 10 pictures to interface
                int iPicsCounter = 0;
                for( int i = 0; i < m_iNbFanPagesFound; i++ )
                {
                    strFanPageId = m_fanPagesFound.at( i );
                    OVFB_MSG( "Getting likes for fanpage %s", strFanPageId.toUtf8().constData() );

                    QList<OVData::FbFriend *> fbFriendList = m_dbHandler->getFanPageFriendListForUserFromDB( strFanPageId, m_strCurrentUserId );

                    if( fbFriendList.isEmpty() )
                    {
                        OVFB_MSG( "No friend is fan of fan page %s", strFanPageId.toUtf8().constData() );
                    }
                    else
                    {
                        foreach( OVData::FbFriend* fbFriend, fbFriendList )
                        {
                            if( !fbFriend->user_id().isNull() && !fbFriend->user_id().isEmpty() )
                            {
                                QString strFriendId = fbFriend->user_id();
                                if( !friendsIds.contains( strFriendId ) )
                                {
                                    friendsIds.append( strFriendId );
                                    if( iPicsCounter < iNbPicsToSend )
                                    {
                                        if( !fbFriend->picture().isNull() && !fbFriend->picture().isEmpty() )
                                        {
                                            QString strPicPath = fbFriend->picture();
                                            OVFB_MSG( "Picture path for fan: %s", strPicPath.toUtf8().constData() );
                                            pics.append( strPicPath );
                                            iPicsCounter++;
                                        }
                                        else
                                        {
                                            OVFB_WARNING( "picture not found in database for friend %s", fbFriend->user_id().toUtf8().constData() );
                                        }
                                    }
                                }
                            }
                            else
                            {
                                OVFB_WARNING( "ID not found in database for friend %s", fbFriend->user_id().toUtf8().constData() );
                            }
                        }
                    }
                }
                int iFanCount = friendsIds.size();
                OVFB_MSG( "%d fan(s) found", iFanCount );

                m_FBInterface->setFriendsLikesNumber( iFanCount );
                m_FBInterface->setFriendsLikesPictures( pics );


                m_strCurrentFanPageId = m_fanPagesFound.at( 0 );
                m_FBInterface->setCurrentFanPageId( m_strCurrentFanPageId );
                OVData::FbFanpage* fbFanPage = m_dbHandler->getFanPageFromDB( m_strCurrentFanPageId );
                if( !fbFanPage )
                {
                    OVFB_MSG( "No fan page %s found in database", m_strCurrentFanPageId.toUtf8().constData() );
                    startRequestsForCategoryFanPage();
                }
                else
                {
                    OVFB_MSG( "Fan page %s found in database.", m_strCurrentFanPageId.toUtf8().constData() );
                    if( fbFanPage->name().isNull() || fbFanPage->name().isEmpty() )
                    {
                        OVFB_MSG( "Fan page %s found in database but without information, retrieving information.", m_strCurrentFanPageId.toUtf8().constData() );
                        startRequestsForCategoryFanPage();
                    }
                    else
                    {
                        setJobTerminated( 1, "search_fanpage" );
                    }
                }
            }
            else if( m_strCurrentJob == "share_message" )
            {
                OVFB_MSG( "Specific actions for job: share_message" );

                if( !m_fbFanPage )
                {
                    m_fbFanPage = OV_NEW(Domaine_App) OVData::FbFanpage();
                    m_fbFanPage->setPage_id( m_fanPagesFound.at( 0 ) );
                    m_fbFanpagePostList = OV_NEW(Domaine_App) OVData::FbFanpagePostList();
                }
                requestFanPageInformation( m_fanPagesFound.at( 0 ) );
            }
            m_strHandleFailedRequestTag = "default";
        }
        else
        {
            if( !m_dbHandler->addContentToDB( fbContent ) )
            {
                OVFB_ERROR( "Error while adding content to database!" );
            }

            OVFB_MSG( "No fan page found!" );
            m_strHandleFailedRequestTag = "default";
            if( m_strCurrentJob == "share_message" )
            {
                requestShareMessage( m_strCurrentUserId, m_strContentToShare, m_strMessageToShare );
            }
            else
            {
                if( m_strCurrentJob == "fbapi__content_likes" )
                {
                    // TODO - FIXME : put this in a function
                    QStringList pics;
                    m_socialMessage.m_iLikeNumber = 0;
                    m_socialMessage.m_listFriendsLike = pics;

                    m_iNbLikesRequestsInProgress -= 1;

                    m_strContentId = "";
                    m_strContentTitle = "";
                    OVFB_MSG( "Sending message to UI - content title: %s; nb likes: %d", m_socialMessage.m_strTitle.toUtf8().constData(), m_socialMessage.m_iLikeNumber );
                    emit sigSendMessageToUI( 0, QVariant().fromValue( m_socialMessage ) );
                    setJobTerminated( -1, "fbapi__content_likes" );
                }
                else
                {
                    setJobTerminated( 2, "search_fanpage" );
                }
            }
        }
    }

    void AppFacebookJob::slotHandleShareMessageRequest()
    {
        QString strPostId = m_facebookLib->getPostId();

        OVFB_IMPORTANT_MSG( "Handling share request - post ID: %s", strPostId.toUtf8().constData() );

        OVFB_MSG("ID of post just posted: %s", strPostId.toUtf8().constData() );

        m_strHandleFailedRequestTag = "default";
        // sleep for 5 secs - allows request to facebook user news feed to contains the just posted post
        sleep( 5 );

        OVFB_MSG( "Adding job 'user_news_feed' to jobs queue." );
        m_jobsQueue.append( "user_news_feed" );
        setJobTerminated( -1, "share_message" );
//        startRequestsForCategoryUserNewsFeed();
    }

}
