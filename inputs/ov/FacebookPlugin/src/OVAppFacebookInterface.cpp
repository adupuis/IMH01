#include <OVAppFacebookInterface.h>

#include <OV3DResourceImage.h>
#include <OV3DResourceManager.h>
#include <OV3DTextureFactory.h>

#include <OVGUIDataImage.h>
#include <OVGUIDataString.h>

#include <OVSchematicsAppPluginContent.h>

#include <OVDataFacebookDebug.h>

#include <OVGUIThemeAccess.h>

namespace OVSchematics
{

    OVAppFacebookInterface::OVAppFacebookInterface()
        : m_pAppFacebookDBHandler( NULL )
        , m_black( 30, 30, 30 )
        , m_orange( 244, 164, 60 )
        , m_snow( 255, 255, 240 )
        , m_green( 143, 188, 143 )
        , m_timeFont( "Helvetica Neue LT Std", 28 , QFont::Normal )
        , m_defaultFont( "Helvetica Neue LT Std", 28, QFont::Bold )
    {
        m_cFocusColor = OVGUI::ThemeAccess::getFocusColor();
    }

    OVAppFacebookInterface::~OVAppFacebookInterface()
    {
        AUTOLOG();

        //don't forget this !!!
        setContentModule( NULL );
        AUTOLOG();

        foreach( AppPluginContent* pCurContent, m_listContent )
        {
            AUTOLOG();
            OV_DELETE pCurContent;
        }

        delete m_pAppFacebookDBHandler;

        AUTOLOG();
    }

    void OVAppFacebookInterface::setAppDatabase( QSqlDatabase* _appDatabase )
    {
        m_pAppDatabase = _appDatabase;
    }

    void OVAppFacebookInterface::setCurrentUserId( QString _strUserId )
    {
        m_strCurrentUserId = _strUserId;
    }

    void OVAppFacebookInterface::setCurrentUserEmail( QString _strCurrentUserEmail )
    {
        m_strCurrentUserEmail = _strCurrentUserEmail;
    }

    void OVAppFacebookInterface::setCurrentUserPwd( QString _strCurrentUserPwd )
    {
        m_strCurrentUserPwd = _strCurrentUserPwd;
    }

    void OVAppFacebookInterface::setCurrentFanPageId( QString _strFanPageId )
    {
        m_strCurrentFanPageId = _strFanPageId;
    }

    void OVAppFacebookInterface::setFriendsLikesPictures( QStringList _friendsLikesPictures )
    {
        m_friendsLikesPictures = _friendsLikesPictures;
    }

    void OVAppFacebookInterface::setFriendsLikesNumber( int _iFriendsLikesNumber )
    {
        m_iFriendsLikesNumber = _iFriendsLikesNumber;
    }

    void OVAppFacebookInterface::slotMessageFromJobReceived( QVariant _message )
    {
        OVFB_MSG( "Message received from Job (%d)", _message.toInt() );
        switch( _message.toInt() )
        {
        case -1:
        {
            // no active user
            if( m_iIndexData == 0 )
            {
                setContentModule( m_listContent[0] );
            }
            m_listContent[0]->clear( true );
            setText( m_listContent[0], "No user logged in", m_snow, m_defaultFont, 1.0, -5.0, 0.5, 0.5, 400, 150 );
            m_listContent[1]->clear( true );
            setText( m_listContent[1], "No user logged in", m_snow, m_defaultFont, 1.0, -5.0, 0.5, 0.5, 400, 150 );
            m_listContent[2]->clear( true );
            setText( m_listContent[2], "No user logged in", m_snow, m_defaultFont, 1.0, -5.0, 0.5, 0.5, 400, 150 );
            m_listContent[3]->clear( true );
            setText( m_listContent[3], "No user logged in", m_snow, m_defaultFont, 1.0, -5.0, 0.5, 0.5, 400, 150 );
        }
            break;
        case 0 :
        {
            // user newsfeed
            setFeedContent( m_pAppFacebookDBHandler, m_listContent[0] );
            setInboxContent( m_pAppFacebookDBHandler, m_listContent[2] );
            if( m_iIndexData == 0 )
            {
                setContentModule( m_listContent[0] );
            }
        }
            break;
        case 1:
        {
            // fan page
            setFanpageContent( m_pAppFacebookDBHandler, m_listContent[1] );
            if( m_iIndexData == 1 )
            {
                setContentModule( m_listContent[1] );
            }
        }
            break;
        case 2:
        {
            // no fan page found
            if( m_iIndexData == 1 )
            {
                setContentModule( m_listContent[1] );
            }
            m_listContent[1]->clear( true );
            setText( m_listContent[1], "No fanpage found!", m_black, m_defaultFont, 2.0, 0.5, 0.5, 0.5, 400, 150 );
        }
            break;
        case 3:
        {
            // fanpage cannot be loaded (no user logged in)
            if( m_iIndexData == 1 )
            {
                setContentModule( m_listContent[1] );
            }
            m_listContent[1]->clear( true );
            setText( m_listContent[1], "No user logged in", m_snow, m_defaultFont, 1.0, -5.0, 0.5, 0.5, 400, 150 );
        }
            break;
        case 4:
        {
            // friendlist
            setFriendlistContent( m_pAppFacebookDBHandler, m_listContent[3] );
            setFanpageContent( m_pAppFacebookDBHandler, m_listContent[1] );
            if( m_iIndexData == 3 )
            {
                setContentModule( m_listContent[3] );
            }
        }
            break;
        case 5:
        {
            // like
            if( m_strCurrentUserEmail.isNull() || m_strCurrentUserEmail.isEmpty() )
            {
                OVFB_ERROR( "No email for current user!" );
            }
            else
            {
                if( m_strCurrentUserPwd.isNull() || m_strCurrentUserPwd.isEmpty() )
                {
                    OVFB_ERROR( "No password for user %s!", m_strCurrentUserEmail.toUtf8().constData() );
                }
                else
                {
                    if( m_fanPagesToLikeOrUnlike.isEmpty() )
                    {
                        OVFB_ERROR( "No fan page to like!" );
                    }
                    else
                    {
                        createSocialBrowser();
                        m_pSocialBrowser->setEmail( m_strCurrentUserEmail );
                        m_pSocialBrowser->setPassword( m_strCurrentUserPwd );
                        m_pSocialBrowser->setFanPagesIds( m_fanPagesToLikeOrUnlike );
                        m_pSocialBrowser->setMakeLike( true );
                        OV_MSG( "Will process facebook login" );
                        m_pSocialBrowser->processFacebookLoginPage();
                    }
                }
            }
        }
            break;
        case 6:
        {
            // unlike
            if( m_strCurrentUserEmail.isNull() || m_strCurrentUserEmail.isEmpty() )
            {
                OVFB_ERROR( "No email for current user!" );
            }
            else
            {
                if( m_strCurrentUserPwd.isNull() || m_strCurrentUserPwd.isEmpty() )
                {
                    OVFB_ERROR( "No password for user %s!", m_strCurrentUserEmail.toUtf8().constData() );
                }
                else
                {
                    if( m_fanPagesToLikeOrUnlike.isEmpty() )
                    {
                        OVFB_ERROR( "No fan page to unlike!" );
                    }
                    else
                    {
                        createSocialBrowser();
                        m_pSocialBrowser->setEmail( m_strCurrentUserEmail );
                        m_pSocialBrowser->setPassword( m_strCurrentUserPwd );
                        m_pSocialBrowser->setFanPagesIds( m_fanPagesToLikeOrUnlike );
                        m_pSocialBrowser->setMakeLike( false );
                        OV_MSG( "Will process facebook login" );
                        m_pSocialBrowser->processFacebookLoginPage();
                    }
                }
            }
        }
            break;
        default:
            break;
        };
    }

    void OVAppFacebookInterface::messageFromUI( int _iID, QVariant _message )
    {
        if( _iID == 1 )
        {
            OVFB_MSG( "Message received for facebook fanpage with content : %s", _message.toString().toUtf8().constData() );
            m_listContent[1]->clear( true );
            if( m_iIndexData == 1 )
            {
                setContentModule( m_listContent[1] );
            }
            setText( m_listContent[1], "Loading fanpage...", m_snow, m_defaultFont, 2.0, -5, 0.5, 0.5, 400, 150 );
            sendMessageToJob( 1, _message );
            setFocusCategoryOnElt( 1 );
        }
        else if( _iID == 3 )
        {
            // Get access token

            sendMessageToJob( 2, "sync_started" );

            QStringList loginPassword( _message.toStringList() );
            QString strLogin = loginPassword.at( 0 ).trimmed();
            QString strPassword = loginPassword.at( 1 ).trimmed();
            OVFB_MSG( "Type: login_password - Login: %s ; Password: %s", strLogin.toUtf8().constData(), strPassword.toUtf8().constData() );

            m_socialEmailAccessToken.insert( 0, strLogin );
            m_socialEmailAccessToken.insert( 1, strPassword );

            createSocialBrowser();
            m_pSocialBrowser->setEmail( strLogin );
            m_pSocialBrowser->setPassword( strPassword );
            OV_MSG( "Will process facebook authentication page..." );
            m_pSocialBrowser->processFacebookOAuthPage();
        }
    }

    void OVAppFacebookInterface::initialInterface()
    {
        AUTOLOG();

        setTitle( "FaceBook-title.png" );

        // Set Background Color on Title & Content
        setBackgroundTitleImg( OV3D::TextureFactory::get( "_customFBColor_0.png" ) );
        setBackgroundContentImg( OV3D::TextureFactory::get( "_customFBColor_1.png" ) );
        setBackgroundCategoryImg( OV3D::TextureFactory::get( "_customFBColor_2.png" ) );

        // Set Menu Icons
        m_catList.push_back( ( OVGUI::Data* ) OV_NEW( Domaine_GUI ) OVGUI::DataImage( OV3D::TextureFactory::get( "FB_icon1_big_passive.png" ) ) );
        m_catList.push_back( ( OVGUI::Data* ) OV_NEW( Domaine_GUI ) OVGUI::DataImage( OV3D::TextureFactory::get( "FB_icon2_big_passive.png" ) ) );
        m_catList.push_back( ( OVGUI::Data* ) OV_NEW( Domaine_GUI ) OVGUI::DataImage( OV3D::TextureFactory::get( "FB_icon3_big_passive.png" ) ) );
        m_catList.push_back( ( OVGUI::Data* ) OV_NEW( Domaine_GUI ) OVGUI::DataImage( OV3D::TextureFactory::get( "FB_icon4_big_passive.png" ) ) );
        m_catList.push_back( ( OVGUI::Data* ) OV_NEW( Domaine_GUI ) OVGUI::DataImage( OV3D::TextureFactory::get( "FB_icon5_big_passive.png" ) ) );

        OVGUI::DataImage* pArrow = OV_NEW( Domaine_GUI ) OVGUI::DataImage( OV3D::TextureFactory::get( "new-arrow-LD.png" ) );
        registerData( * pArrow );

        m_pArrowCat = ( OVGUI::GUIImage* )pArrow->createGUIObject( "arrowFacebookCat" );
        m_pArrowCat->setVisible( true );
        m_pArrowCat->setPosition( Vectormath::Aos::Vector3( 0.7, 0.0, 0.0 ) );
        m_pArrowCat->setRotation( Vectormath::Aos::Quat::rotation( -M_PI/2, Vectormath::Aos::Vector3::zAxis() ) );
        m_pArrowCat->setScale( Vectormath::Aos::Vector3( 0.8, 0.8 / ( 256/97 ), 1 ) );
        registerMovable( * m_pArrowCat );

        //set database
        m_pAppFacebookDBHandler = OV_NEW( Domaine_App ) OVAppFacebookDBHandler;
        m_pAppFacebookDBHandler->setAppDatabase( m_pAppDatabase );

        // category 1: user newsfeed
        AppPluginContent* pCurContent = OV_NEW( Domaine_GUI ) AppPluginContent();
        setFeedContent( m_pAppFacebookDBHandler, pCurContent );
        m_listContent.push_back( pCurContent );

        // category 2: fanpage
        pCurContent = OV_NEW( Domaine_GUI ) AppPluginContent();
        setFanpageContent( m_pAppFacebookDBHandler, pCurContent );
        m_listContent.push_back( pCurContent );

        // category 3: Direct Messages list -- Inbox
        pCurContent = OV_NEW( Domaine_GUI ) AppPluginContent();
        setInboxContent( m_pAppFacebookDBHandler, pCurContent );
        m_listContent.push_back( pCurContent );

        // category 4: friendlist
        pCurContent = OV_NEW( Domaine_GUI ) AppPluginContent();
        setFriendlistContent( m_pAppFacebookDBHandler, pCurContent );
        m_listContent.push_back( pCurContent );

        //category 5: profile
        pCurContent = OV_NEW( Domaine_GUI ) AppPluginContent();
        /*
        setEventPicture(pCurContent, "fb_event1.jpg", 1.0, -1.0);
        setPostContent(pCurContent, "Vin Lee and 22 other guests", 2.2, -0.0);
        setPostTime(pCurContent, "Tomorrow 9:00am", 2.2, -1.0);
        setPostContent(pCurContent, "Graduation Photo With ME !!!!! =)", 2.2, -2.0);

        setEventPicture(pCurContent, "fb_event2.jpg", 1.0, -5.0);
        setPostUserName(pCurContent, "Paul Phillips and 4 other guests", 2.2, -4.0);
        setPostTime(pCurContent, "Saturday 7:00pm", 2.2, -5.0);
        setPostContent(pCurContent, "Posh Flat Party", 2.2, -6.0);
        */
        m_listContent.push_back( pCurContent );

        //category 5: Friends
        pCurContent = OV_NEW( Domaine_GUI ) AppPluginContent();
        /*
        setPostUserPicture(pCurContent, "pic_twitter-following-default-7.png", 1.0, -1.0);
        setPostUserName(pCurContent, "Akrosv", 2.2, 0.0);
        setPostContent(pCurContent, "Profile Pictures", 2.2, -1.0);
        */
        m_listContent.push_back( pCurContent );

        registerData( m_catList );
        setDataCategoryList( m_catList, Vectormath::Aos::Vector3( 0.0, -1.1, 0.0 ), Vectormath::Aos::Vector3( 1.5, 1.5, 1.0 ) );
        setContentModule( m_listContent[0] );

        AUTOLOG();

    }

//    void OVAppFacebookInterface::setFanPageToLikeOrUnlike( QString _strIdOfFanPageToLikeOrUnlike )
//    {
//        m_strIdOfFanPageToLikeOrUnlike = _strIdOfFanPageToLikeOrUnlike;
//    }

    void OVAppFacebookInterface::setFanPagesToLikeOrUnlike( QStringList _fanPagesToLikeOrUnlike )
    {
        m_fanPagesToLikeOrUnlike = _fanPagesToLikeOrUnlike;
    }

    void OVAppFacebookInterface::categoryChanged( OVGUI::GUIObject* _oldObjectSelected, OVGUI::GUIObject* _objectSelected )
    {
        if( _objectSelected )
        {
            _objectSelected->setShadow( 1.5 );
            m_pArrowCat->setParentNode( & _objectSelected->getScaleNode() );
        }
        if( _oldObjectSelected )
        {
            _oldObjectSelected->setShadow( 1.0 );
        }

        const OVGUI::Data* pCurData = & _objectSelected->getData();
        m_iIndexData = -1;
        int iCurIndex = 0;
        foreach( OVGUI::Data* pCatData, m_catList )
        {
            if( pCatData == pCurData )
            {
                m_iIndexData = iCurIndex;
                break;
            }
            iCurIndex++;
        }

        OV_ASSERT( m_iIndexData != -1 && m_listContent.size() > m_iIndexData );

        setContentModule( m_listContent[m_iIndexData] );
    }

    void OVAppFacebookInterface::setFeedContent( OVAppFacebookDBHandler* _appFacebookDBHandler, AppPluginContent* _pCurContent )
    {
        _pCurContent->clear( true );
        m_pAppFacebookDBHandler->setCurrentUserId( m_strCurrentUserId );

        if( m_strCurrentUserId.isEmpty() )
        {
            setText( _pCurContent, "No user logged in", m_snow, m_defaultFont, 1.0, -5.0, 0.5, 0.5, 400, 150 );
        }
        else
        {
            // user name
            double x = 8.0, y = 0.0;
            setPicture( _pCurContent, _appFacebookDBHandler->getCurrentUserPicturePath(), x, y, 2.0, 2.0 );
            x -= 7.5;
            setText( _pCurContent, _appFacebookDBHandler->getCurrentUserName(), m_orange, m_defaultFont, x, y, 0.6, 0.6, 270, 100 );

            // newsfeed
            x = 2.0, y -= 2.0;
            setText( _pCurContent, "Newsfeed", m_black, m_defaultFont, x, y, 0.8, 0.8, 200, 70 );

            QStringList userFeedPostsList = _appFacebookDBHandler->getCurrentUserFeedPostsList();
            y -= 3.5;

            for( int i = 0; i < userFeedPostsList.size(); ++i )
            {
                x = 1.0;
                setPicture( _pCurContent, _appFacebookDBHandler->getCurrentUserFeedPostUserPicturePath( userFeedPostsList.at( i ) ), x, y, 2.0, 2.0 );

                x += 1.4, y += 0.8;
                QString strUserFeedPostUserName = _appFacebookDBHandler->getCurrentUserFeedPostUserName( userFeedPostsList.at( i ) );
                setText( _pCurContent, strUserFeedPostUserName, m_orange, m_defaultFont, x, y, 0.5, 0.5, 550, 50 );

                y -= 0.7;
                QString strUserFeedPostTime =  _appFacebookDBHandler->getCurrentUserFeedPostTime( userFeedPostsList.at( i ) );
                setText( _pCurContent, strUserFeedPostTime, m_green, m_timeFont, x, y, 0.5, 0.5, 550, 50 );

                x -= 2.4, y -= 1.4;
                setText( _pCurContent, _appFacebookDBHandler->getCurrentUserFeedPostMessage( userFeedPostsList.at( i ) ), m_snow, m_timeFont, x, y, 0.5, 0.5, 500, 150 );
                y -= 4.5;
            }
        }
    }

    void OVAppFacebookInterface::setFanpageContent( OVAppFacebookDBHandler* _appFacebookDBHandler, AppPluginContent* _pCurContent )
    {
        _pCurContent->clear( true );

        if( m_strCurrentUserId.isEmpty() )
        {
            setText( _pCurContent, "No user logged in", m_snow, m_defaultFont, 1.0, -5.0, 0.5, 0.5, 400, 150 );
        }
        else
        {
            double x = 2.0, y = 0.5;
            setText( _pCurContent, "Fanpage", m_black, m_defaultFont, x, y, 0.8, 0.8, 200, 50 );

            if( m_strCurrentFanPageId.isNull() || m_strCurrentFanPageId.isEmpty() )
            {
                OVFB_MSG( "I cannot display fanpage as there isn't any fan page to load!" );
                x = 1.0, y = -5.0;
                setText( _pCurContent, "No fanpage to display", m_snow, m_defaultFont, x, y, 0.5, 0.5, 400, 150 );
            }
            else
            {
                m_pAppFacebookDBHandler->setCurrentFanPageId( m_strCurrentFanPageId );

                // title
                x = 0.0, y -= 1.5;
                QString strCurrentFanPageName = _appFacebookDBHandler->getCurrentFanPageName();
                if( !strCurrentFanPageName.isNull() && !strCurrentFanPageName.isEmpty() )
                {
                    setText( _pCurContent, strCurrentFanPageName, m_orange, m_defaultFont, x, y, 0.8, 0.8, 400, 150 );
                }
                // image
                x = 1.7, y -= 6.0;
                QString strCurrentFanPagePicturePath = _appFacebookDBHandler->getCurrentFanPagePicturePath();
                if( !strCurrentFanPagePicturePath.isNull() && !strCurrentFanPagePicturePath.isEmpty() )
                {
                    setPicture( _pCurContent, strCurrentFanPagePicturePath, x, y, 4.5, 6.0 );
                }
                // director, actor and outline
                x = 4.2, y += 2.5;
                QString strCurrentFanPageDirectedBy = _appFacebookDBHandler->getCurrentFanPageDirectedBy();
                if( !strCurrentFanPageDirectedBy.isNull() && !strCurrentFanPageDirectedBy.isEmpty() )
                {
                    QString strDirector = "Directed By: ";
                    strDirector.append( strCurrentFanPageDirectedBy );
                    setText( _pCurContent, strDirector, m_snow,m_timeFont, x, y, 0.5, 0.5, 300, 100 );
                }

                y -= 2.0;
                QString strCurrentFanPageStarring = _appFacebookDBHandler->getCurrentFanPageStarring();
                if( !strCurrentFanPageStarring.isNull() && !strCurrentFanPageStarring.isEmpty() )
                {
                    QString strActor = "Starring: ";
                    strActor.append( strCurrentFanPageStarring );
                    setText( _pCurContent, strActor, m_snow, m_timeFont, x, y, 0.5, 0.5, 300, 200 );
                }

                x = -0.4, y -= 4.0;
                QString strCurrentFanPagePlotOutline = _appFacebookDBHandler->getCurrentFanPagePlotOutline();
                if( !strCurrentFanPagePlotOutline.isNull() && !strCurrentFanPagePlotOutline.isEmpty() )
                {
                    setText( _pCurContent, strCurrentFanPagePlotOutline, m_snow, m_timeFont, x, y, 0.5, 0.5, 550, 250 );
                    y -= 5.0;
                }

                // people likes
    //            y-=5.0;
                QString strPeopleLikeThis = QString().setNum( _appFacebookDBHandler->getCurrentFanPageFanCount() );
                strPeopleLikeThis = insertSpacesIntoNumber( strPeopleLikeThis );
                strPeopleLikeThis.append( " people like this" );
                setText( _pCurContent, strPeopleLikeThis, m_black, m_defaultFont, x, y, 0.5, 0.5, 550, 50 );

                // friends likes
                y -= 0.8;
                QString strFriendsLikes = QString::number( m_iFriendsLikesNumber );
                if( m_iFriendsLikesNumber <= 0 )
                {
                    strFriendsLikes = "no friend like this";
                }
                else if( m_iFriendsLikesNumber == 1 )
                {
                    strFriendsLikes.append(" friend like this:");
                }
                else
                {
                    strFriendsLikes.append(" friends like this:");
                }
                setText( _pCurContent, strFriendsLikes, m_black, m_defaultFont, x, y, 0.5, 0.5, 550, 50 );

                x = 0.3, y -= 1.2;
                for( int i = 0; i < m_friendsLikesPictures.size(); ++i )
                {
                    setPicture( _pCurContent, m_friendsLikesPictures.at( i ), x, y, 1.0, 1.0 );
                    x += 1.2;
                }

                // comments:
                x = -0.4, y -= 1.5;
                setText( _pCurContent, "Comments:", m_black, m_defaultFont, x, y, 0.5, 0.5, 550, 50 );
                QStringList fanPagePostsList = _appFacebookDBHandler->getCurrentFanPagePostsList();
                y -= 1.5;
                for( int i = 0; i < fanPagePostsList.size(); ++i )
                {
                    x = 0.1;
                    setPicture( _pCurContent, _appFacebookDBHandler->getCurrentFanPagePostUserPicturePath( fanPagePostsList.at( i ) ), x, y, 1.0, 1.0 );

                    x += 0.7, y += 0.4;
                    QString postUserName = _appFacebookDBHandler->getCurrentFanPagePostUserName( fanPagePostsList.at( i ) );
                    setText( _pCurContent, postUserName, m_orange, m_timeFont, x, y, 0.5, 0.5, 550, 50 );

                    y -= 0.7;
                    QString postTime =  _appFacebookDBHandler->getCurrentFanPagePostTime( fanPagePostsList.at( i ) );
                    setText( _pCurContent, postTime, m_green, m_timeFont, x, y, 0.5, 0.5, 550, 50 );

                    x -= 1.2, y -= 0.8;
                    setText( _pCurContent, _appFacebookDBHandler->getCurrentFanPagePostMessage( fanPagePostsList.at( i ) ), m_snow, m_timeFont, x, y, 0.5, 0.5, 500, 150 );
                    y -= 4.0;
                }
            }
        }
    }

    void OVAppFacebookInterface::setInboxContent( OVAppFacebookDBHandler* _appFacebookDBHandler, AppPluginContent* _pCurContent )
    {
        _pCurContent->clear( true );
        m_pAppFacebookDBHandler->setCurrentUserId( m_strCurrentUserId );

        if( m_strCurrentUserId.isEmpty() )
        {
            setText( _pCurContent, "No user logged in", m_snow, m_defaultFont, 1.0, -5.0, 0.5, 0.5, 400, 150 );
        }
        else
        {
            double x = 2.0, y = 0.5;
            // article name
            QString strName = "Inbox (";
            QString nbMail =  QString().setNum( 1 );
            setText( _pCurContent, strName.append( nbMail.append( ")" ) ), m_black, m_defaultFont, x, y, 0.8, 0.8, 200, 70 );

            for( int i = 0; i < 1; ++i )
            {
                // user picture
                x -= 1.0, y -= 3.0;
                setPicture( _pCurContent, _appFacebookDBHandler->getCurrentUserPicturePath(), x, y, 2.0, 2.0 );
                // user name
                x += 1.2, y += 1.0;
                setText( _pCurContent, _appFacebookDBHandler->getCurrentUserName(), m_orange, m_defaultFont, x, y, 0.5, 0.5, 400, 50 );
                // message subject
                y -= 0.7;
                setText( _pCurContent, "Come soon~", m_black, m_defaultFont, x, y, 0.6, 0.6, 400, 50 );
                // post time
                y -= 0.7;
                setText( _pCurContent, "yesterday 02:56pm", m_green, m_timeFont, x, y, 0.5, 0.5, 400, 50 );
                // pictures
                x -= 0.9, y -= 1.5;
                setPicture( _pCurContent, "reply1.png", x, y, 2.0, 0.7 );
                x += 2.5;
                setPicture( _pCurContent, "forward1.png", x, y, 2.3, 0.7 );
                // post message
                x -= 4.0, y -= 1.0;
                setText( _pCurContent, "Birthday party in office for our south Africa colleague ! Yeah food & beer~", m_snow, m_timeFont, x, y, 0.5, 0.5, 500, 150 );

                y -= 2.0;
            }
        }
    }

    void OVAppFacebookInterface::setFriendlistContent( OVAppFacebookDBHandler* _appFacebookDBHandler, AppPluginContent* _pCurContent )
    {
        _pCurContent->clear( true );
        m_pAppFacebookDBHandler->setCurrentUserId( m_strCurrentUserId );

        if( m_strCurrentUserId.isEmpty() )
        {
            setText( _pCurContent, "No user logged in", m_snow, m_defaultFont, 1.0, -5.0, 0.5, 0.5, 400, 150 );
        }
        else
        {
            // user name
            double x = 8.0, y = 0.0;
            setPicture( _pCurContent, _appFacebookDBHandler->getCurrentUserPicturePath(), x, y, 2.0, 2.0 );
            x -= 7.5;
            setText( _pCurContent, _appFacebookDBHandler->getCurrentUserName(), m_orange, m_defaultFont, x, y, 0.6, 0.6, 270, 100 );

            // friendlist
            x = 2.0, y -= 2.0;
            setText( _pCurContent, "Friends", m_black, m_defaultFont, x, y, 0.8, 0.8, 200, 70 );

            QStringList userFriendList = _appFacebookDBHandler->getCurrentUserFriendList();
            y -= 3.5;

            for( int i = 0; i < userFriendList.size(); ++i )
            {
                x = 1.0;
                setPicture( _pCurContent, _appFacebookDBHandler->getFriendPicturePath( userFriendList.at( i ) ), x, y, 1.0, 1.0 );

                x += 1.4;
                QString strFriendName = _appFacebookDBHandler->getFriendName( userFriendList.at( i ) );
                setText( _pCurContent, strFriendName, m_snow, m_defaultFont, x, y, 0.4, 0.4, 550, 50 );

                y -= 1.5;
            }
        }
    }

    void OVAppFacebookInterface::setPicture( AppPluginContent* _pCurContent, QString _strPictureFilepath, double _fX, double _fY, double _fW, double _fH )
    {
        // Check if image really exists on disk (NFS sync issue?)
        if( !OV3D::ResourceManager::instance().exists( _strPictureFilepath ) )
        {
            OVFB_ERROR( "Picture not found on disk! filepath: %s", _strPictureFilepath.toUtf8().constData() );
        }
        else
        {
            _pCurContent->addImage( OV_NEW( Domaine_GUI ) OVGUI::DataImage( OV3D::TextureFactory::get( _strPictureFilepath ) ),
                Vectormath::Aos::Vector3( _fX, _fY, 0.0 ), Vectormath::Aos::Vector3( _fW, _fH, 1.0 ) );
        }
    }

    void OVAppFacebookInterface::setText( AppPluginContent* _pCurContent, QString _strText, QColor _color, QFont _font, double _fX, double _fY, double _fW, double _fH, int _iCanvasWidth, int _iCanvasHeight )
    {
        // set default point size so that it becomes sexyyyyy // TODO: check why it modifies canvas
//            font.setPointSize( 64 );

        _pCurContent->setTextColor( _color );
        _pCurContent->setTextFont( _font );
        _pCurContent->addText( OV_NEW( Domaine_GUI ) OVGUI::DataString( _strText ),
            Vectormath::Aos::Vector3( _fX, _fY, 1.0 ), Vectormath::Aos::Vector3( _fW, _fH, 1.0 ), Vectormath::Aos::Quat::identity(), QSize( _iCanvasWidth, _iCanvasHeight ) );

    }

    void OVAppFacebookInterface::onFocusCategory()
    {
            m_pArrowCat->setColor( m_cFocusColor );
    }

    void OVAppFacebookInterface::onFocusContent()
    {
           m_pArrowCat->setColor( QColor( 155, 155, 155, 255 ) );
    }

    QString OVAppFacebookInterface::insertSpacesIntoNumber( QString & _strNumber )
    {
        if( _strNumber.size() > 7 )
        {
            _strNumber.insert( 2, " " );
            _strNumber.insert( 6, " " );
        }
        else if( _strNumber.size() > 6 )
        {
            _strNumber.insert( 1, " " );
            _strNumber.insert( 5, " " );
        }
        else if( _strNumber.size() > 5 )
        {
            _strNumber.insert( 3, " " );
        }
        else if( _strNumber.size() > 4 )
        {
            _strNumber.insert( 2, " " );
        }
        else if( _strNumber.size() > 3 )
        {
            _strNumber.insert( 1, " " );
        }
        return _strNumber;
    }

    void OVAppFacebookInterface::createSocialBrowser()
    {
        OV_MSG( "Creating social browser..." );

        m_pSocialBrowser = OV_NEW( Domaine_App ) OVSocialBrowser;

        OV_CONNECT( m_pSocialBrowser,  SIGNAL( sigOAuthActionFailed( const int, const QString& ) ),
                    this,              SLOT(   slotHandleOAuthActionFailed( const int, const QString& ) ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_pSocialBrowser,  SIGNAL( sigLikeActionFailed( const int, const QString& ) ),
                    this,              SLOT(   slotHandleLikeActionFailed( const int, const QString& ) ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_pSocialBrowser,  SIGNAL( sigUnlikeActionFailed( const int, const QString& ) ),
                    this,              SLOT(   slotHandleUnlikeActionFailed( const int, const QString& ) ),
                   Qt::UniqueConnection );


        OV_CONNECT( m_pSocialBrowser,  SIGNAL( sigAccessTokenRetrieved( const QString& ) ),
                    this,             SLOT(   slotHandleAccessTokenRetrieved( const QString& ) ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_pSocialBrowser,  SIGNAL( sigLikeActionDone() ),
                    this,             SLOT(   slotHandleLikeActionDone() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_pSocialBrowser,  SIGNAL( sigUnlikeActionDone() ),
                    this,             SLOT(   slotHandleUnlikeActionDone() ),
                   Qt::UniqueConnection );

        OV_CONNECT( m_pSocialBrowser,  SIGNAL( destroyed() ),
                    this,             SLOT(   slotHandleSocialBrowserDestroyed() ),
                   Qt::UniqueConnection );
    }

    void OVAppFacebookInterface::destroySocialBrowser()
    {
        OV_MSG( "Destroying social browser..." );
        m_pSocialBrowser->deleteLater();
    }

    void OVAppFacebookInterface::slotHandleOAuthActionFailed( const int _iId, const QString& _strErrorMessage )
    {
        OV_MSG( "Social Browser OAuth action failed, destroying social browser." );
        destroySocialBrowser();

        QStringList oAuthError;
        oAuthError.append( QString::number( _iId ) );
        oAuthError.append( _strErrorMessage );
        OVFB_MSG( "Sending message to job - social browser oauth action failure - id: %d ; error message: %s", _iId, _strErrorMessage.toUtf8().constData() );
        sendMessageToJob( 3, oAuthError );
    }

    void OVAppFacebookInterface::slotHandleLikeActionFailed( const int _iId, const QString& _strErrorMessage )
    {
        OV_MSG( "Social Browser Like action failed, destroying social browser." );
        destroySocialBrowser();

        QStringList likeError;
        likeError.append( QString::number( _iId ) );
        likeError.append( _strErrorMessage );
        OVFB_MSG( "Sending message to job - social browser like action failure - id: %d ; error message: %s", _iId, _strErrorMessage.toUtf8().constData() );
        sendMessageToJob( 4, likeError );
    }

    void OVAppFacebookInterface::slotHandleUnlikeActionFailed( const int _iId, const QString& _strErrorMessage )
    {
        OV_MSG( "Social Browser unlike action failed, destroying social browser." );
        destroySocialBrowser();

        QStringList unlikeError;
        unlikeError.append( QString::number( _iId ) );
        unlikeError.append( _strErrorMessage );
        OVFB_MSG( "Sending message to job - social browser unlike action failure - id: %d ; error message: %s", _iId, _strErrorMessage.toUtf8().constData() );
        sendMessageToJob( 4, unlikeError );
    }

    void OVAppFacebookInterface::slotHandleAccessTokenRetrieved( const QString& _strAccessToken )
    {
        m_socialEmailAccessToken.insert( 2, _strAccessToken );

        destroySocialBrowser();

        OVFB_MSG( "Sending message to job - email: %s - access_token: %s", m_socialEmailAccessToken.at( 0 ).toUtf8().constData(), m_socialEmailAccessToken.at( 2 ).toUtf8().constData() );
        sendMessageToJob( 0, m_socialEmailAccessToken );
    }

    void OVAppFacebookInterface::slotHandleLikeActionDone()
    {
        OVFB_MSG( "Like action Done! Destroying Social Browser..." );

        destroySocialBrowser();

        sendMessageToJob( 5, 0 );
    }

    void OVAppFacebookInterface::slotHandleUnlikeActionDone()
    {
        OVFB_MSG( "Unlike action Done! Destroying Social Browser..." );

        destroySocialBrowser();

        sendMessageToJob( 6, 0 );
    }

    void OVAppFacebookInterface::slotHandleSocialBrowserDestroyed()
    {
        OVFB_IMPORTANT_MSG( "Social Browser destroyed!" );
    }

}
