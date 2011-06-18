#include <OVAppFacebookDBHandler.h>
#include <OVAppFacebookJob.h>

#include <OVDataFacebookDebug.h>

#include <OVDebugCommon.h>


namespace OVSchematics
{
    OVAppFacebookDBHandler::OVAppFacebookDBHandler()
    {
//        QMutex::Recursive
        m_appDatabase = NULL;
    }

    OVAppFacebookDBHandler::~OVAppFacebookDBHandler()
    {
//        delete m_appDatabase;
    }


    // Getters, Setters


    void OVAppFacebookDBHandler::setAppDatabase( QSqlDatabase* _appDatabase )
    {
        m_appDatabase = _appDatabase;
    }


    QString OVAppFacebookDBHandler::getPicturesFilepath()
    {
        return m_strPicturesFilepath;
    }

    void OVAppFacebookDBHandler::setPicturesFilepath( QString _strFilepath )
    {
        m_strPicturesFilepath = _strFilepath;
    }


    QString OVAppFacebookDBHandler::getCurrentUserId()
    {
        return m_strCurrentUserId;
    }

    void OVAppFacebookDBHandler::setCurrentUserId( QString _strUserId )
    {
        m_strCurrentUserId = _strUserId;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPageId()
    {
        return m_strCurrentFanPageId;
    }

    void OVAppFacebookDBHandler::setCurrentFanPageId( QString _strFanPageId )
    {
        m_strCurrentFanPageId = _strFanPageId;
    }




    // Methods used by plugin UI to get Facebook data


    QString OVAppFacebookDBHandler::getCurrentUserName()
    {
        OVFB_MSG("Getting user name for user %s", m_strCurrentUserId.toUtf8().constData() );

        QString strUserName = "No user logged in";

        OVData::FbUser* fbUser = getUserFromDB( m_strCurrentUserId );
        if( !fbUser )
        {
            OVFB_MSG( "No user found!" );
        }
        else
        {
            if( fbUser->name().isNull() || fbUser->name().isEmpty() )
            {
                OVFB_WARNING("No user name found in database!");
            }
            else
            {
                strUserName = fbUser->name();
//                OVFB_MSG("user name found: %s", strUserName.toUtf8().constData() );
            }
        }

        return strUserName;
    }

    QString OVAppFacebookDBHandler::getCurrentUserPicturePath()
    {
        OVFB_MSG("Getting user picture for user %s", m_strCurrentUserId.toUtf8().constData() );

        QString strUserPictureFilename = "no_avatar.png";

        OVData::FbUser* fbUser = getUserFromDB( m_strCurrentUserId );
        if( !fbUser )
        {
            OVFB_MSG( "No user found!" );
        }
        else
        {
            if( fbUser->picture().isNull() || fbUser->picture().isEmpty() )
            {
                OVFB_WARNING("No user picture found in database!");
            }
            else
            {
                strUserPictureFilename = fbUser->picture();
//                OVFB_MSG("user picture path found: %s", strUserPictureFilename.toUtf8().constData() );
            }
        }

        return strUserPictureFilename;
    }

    QStringList OVAppFacebookDBHandler::getCurrentUserFriendList()
    {
        OVFB_MSG("Getting user friendlist for user %s", m_strCurrentUserId.toUtf8().constData() );

        QStringList userFriendList;
        QMap<QString, QString> userFriendListMap;

        OVData::FbUserFriendList* fbUserFriendList = getUserFriendListFromDB( m_strCurrentUserId );
        if( !fbUserFriendList )
        {
            OVFB_MSG( "No user friendlist found" );
        }
        else
        {
            foreach( OVData::FbFriend* fbFriend, fbUserFriendList->friends() )
            {
                if( fbFriend->user_id().isNull() || fbFriend->user_id().isEmpty() )
                {
                    OVFB_WARNING("No friend found in database!");
                }
                else
                {
//                    userFriendList.append( fbFriend->user_id() );
                    userFriendListMap[fbFriend->name()] = fbFriend->user_id();
                }
            }

            QMapIterator<QString, QString> i( userFriendListMap );
            while( i.hasNext() )
            {
                i.next();
//                OVFB_MSG( "friend name: %s - id: %s", i.key().toUtf8().constData(), i.value().toUtf8().constData() );
                userFriendList.append( i.value() );
            }
        }

        return userFriendList;
    }

    QString OVAppFacebookDBHandler::getFriendName( QString _strFriendUserId )
    {
        OVFB_MSG("Getting user name of friend %s", _strFriendUserId.toUtf8().constData() );

        QString strFriendName = "Bill Bontham";

        OVData::FbFriend* fbFriend = getFriendFromDB( _strFriendUserId );
        if( !fbFriend )
        {
            OVFB_MSG( "No friend found!" );
        }
        else
        {
            if( fbFriend->name().isNull() || fbFriend->name().isEmpty() )
            {
                OVFB_WARNING("No friend name found in database!");
            }
            else
            {
                strFriendName = fbFriend->name();
//                OVFB_MSG("friend name found: %s", strFriendName.toUtf8().constData() );
            }
        }

        return strFriendName;
    }

    QString OVAppFacebookDBHandler::getFriendPicturePath( QString _strFriendUserId )
    {
        OVFB_MSG("Getting user picture of friend %s", _strFriendUserId.toUtf8().constData() );

        QString strFriendPictureFilename = "no_avatar.png";

        OVData::FbFriend* fbFriend = getFriendFromDB( _strFriendUserId );
        if( !fbFriend )
        {
            OVFB_MSG( "No user found!" );
        }
        else
        {
            if( fbFriend->picture().isNull() || fbFriend->picture().isEmpty() )
            {
                OVFB_WARNING("No friend picture found in database!");
            }
            else
            {
                strFriendPictureFilename = fbFriend->picture();
//                OVFB_MSG("friend picture path found: %s", strFriendPictureFilename.toUtf8().constData() );
            }
        }

        return strFriendPictureFilename;
    }

    QStringList OVAppFacebookDBHandler::getCurrentUserFeedPostsList()
    {
        OVFB_MSG("Getting user feed posts list for user %s", m_strCurrentUserId.toUtf8().constData() );

        QStringList userFeedPostsList;
        QMap<QString, QString> userFeedPostsListMap;

        OVData::FbUserfeedpostList* fbUserFeedPostList = getUserFeedPostListFromDB( m_strCurrentUserId );
        if( !fbUserFeedPostList )
        {
            OVFB_MSG( "No user feed post list found" );
        }
        else
        {
            foreach( OVData::FbUserfeedpost* fbUserFeedPost, fbUserFeedPostList->posts() )
            {
                if( fbUserFeedPost->post_id().isNull() || fbUserFeedPost->post_id().isEmpty() )
                {
                    OVFB_WARNING("No user feed post found in database!");
                }
                else
                {
                    userFeedPostsListMap[fbUserFeedPost->time().toString( Qt::ISODate )] = fbUserFeedPost->post_id();
//                    OVFB_MSG("user feed time found: %s", fbUserFeedPost->time().toString( Qt::ISODate ).toUtf8().constData() );
//                    OVFB_MSG("user feed post id found: %s", fbUserFeedPost->post_id().toUtf8().constData() );
                }
            }

            QMapIterator<QString, QString> i( userFeedPostsListMap );
            i.toBack();
            while( i.hasPrevious() )
            {
                i.previous();
                userFeedPostsList.append( i.value() );
            }
        }

        return userFeedPostsList;
    }

    QString OVAppFacebookDBHandler::getCurrentUserFeedPostUserName( QString _strUserFeedPostId )
    {
        OVFB_MSG("Getting user feed post user name for post %s", _strUserFeedPostId.toUtf8().constData() );

        QString strUserFeedPostUserName = "Adam Nichols";

        OVData::FbUserfeedpost* fbUserFeedPost = getUserFeedPostFromDB( _strUserFeedPostId );
        if( !fbUserFeedPost )
        {
            OVFB_MSG( "No user post found!" );
        }
        else
        {
            if( getFriendName( fbUserFeedPost->user_id() ).isNull() || getFriendName( fbUserFeedPost->user_id() ).isEmpty() )
            {
                OVFB_WARNING("No feed post user name found in database!");
            }
            else
            {
                strUserFeedPostUserName = getFriendName( fbUserFeedPost->user_id() );
//                OVFB_MSG("user feed post user name found: %s", strUserFeedPostUserName.toUtf8().constData() );
            }
        }

        return strUserFeedPostUserName;
    }

    QString OVAppFacebookDBHandler::getCurrentUserFeedPostUserPicturePath( QString _strUserFeedPostId )
    {
        OVFB_MSG("Getting user feed post user picture for post %s", _strUserFeedPostId.toUtf8().constData() );

        QString strUserFeedPostUserPictureFilename = "no_avatar.png";

        OVData::FbUserfeedpost* fbUserFeedPost = getUserFeedPostFromDB( _strUserFeedPostId );
        if( !fbUserFeedPost )
        {
            OVFB_MSG( "No user feed post found!" );
        }
        else
        {
            if( getFriendPicturePath( fbUserFeedPost->user_id() ).isNull() || getFriendPicturePath( fbUserFeedPost->user_id() ).isEmpty() )
            {
                OVFB_WARNING("No feed post user picture found in database!");
            }
            else
            {
                strUserFeedPostUserPictureFilename = getFriendPicturePath( fbUserFeedPost->user_id() );
//                OVFB_MSG("user feed post user picture found: %s", strUserFeedPostUserPictureFilename.toUtf8().constData() );
            }
        }
        return strUserFeedPostUserPictureFilename;
    }

    QString OVAppFacebookDBHandler::getCurrentUserFeedPostTime( QString _strUserFeedPostId )
    {
        OVFB_MSG("Getting user feed post time for post %s", _strUserFeedPostId.toUtf8().constData() );

        QString strUserFeedPostTime = "2010-11-08 16:42:02";

        OVData::FbUserfeedpost* fbUserFeedPost = getUserFeedPostFromDB( _strUserFeedPostId );
        if( !fbUserFeedPost )
        {
            OVFB_MSG( "No user feed post found!" );
        }
        else
        {
            if( fbUserFeedPost->time().toString().isNull() || fbUserFeedPost->time().toString().isEmpty() )
            {
                OVFB_WARNING("No feed post time found in database!");
            }
            else
            {
                strUserFeedPostTime = fbUserFeedPost->time().toString();
//                OVFB_MSG("user feed post time found: %s", strUserFeedPostTime.toUtf8().constData() );
            }
        }

        return strUserFeedPostTime;
    }

    QString OVAppFacebookDBHandler::getCurrentUserFeedPostMessage( QString _strUserFeedPostId )
    {
        OVFB_MSG("Getting user feed post message for post %s", _strUserFeedPostId.toUtf8().constData() );

        QString strUserFeedPostMessage = "Hey !";

        OVData::FbUserfeedpost* fbUserFeedPost = getUserFeedPostFromDB( _strUserFeedPostId );
        if( !fbUserFeedPost )
        {
            OVFB_MSG( "No user feed post found!" );
        }
        else
        {
            if( fbUserFeedPost->message().isNull() || fbUserFeedPost->message().isEmpty() )
            {
                OVFB_WARNING("No feed post message found in database!");
            }
            else
            {
                strUserFeedPostMessage = fbUserFeedPost->message();
//                OVFB_MSG("user feed post time found: %s", strUserFeedPostMessage.toUtf8().constData() );
            }
        }

        return strUserFeedPostMessage;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPageName()
    {
        OVFB_MSG("Getting name for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        QString strFanPageName = "";

        OVData::FbFanpage* fbFanPage = getFanPageFromDB( m_strCurrentFanPageId );
        if( !fbFanPage )
        {
            OVFB_MSG( "No fan page found!" );
        }
        else
        {
            if( fbFanPage->name().isNull() || fbFanPage->name().isEmpty() )
            {
                OVFB_WARNING("No fan page name found in database!");
            }
            else
            {
                strFanPageName = fbFanPage->name();
//                OVFB_MSG("fan page name found: %s", strFanPageName.toUtf8().constData() );
            }
        }

        return strFanPageName;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPagePicturePath()
    {
        OVFB_MSG("Getting picture for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        QString strFanPagePictureFilename = "no_poster.jpg";

        OVData::FbFanpage* fbFanPage = getFanPageFromDB( m_strCurrentFanPageId );
        if( !fbFanPage )
        {
            OVFB_MSG( "No fan page found!" );
        }
        else
        {
            if( fbFanPage->picture().isNull() || fbFanPage->picture().isEmpty() )
            {
                OVFB_WARNING("No fan page picture found in database!");
            }
            else
            {
                strFanPagePictureFilename = fbFanPage->picture();
//                OVFB_MSG("fan page picture found: %s", strFanPagePictureFilename.toUtf8().constData() );
            }
        }

        return strFanPagePictureFilename;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPageLink()
    {
        OVFB_MSG("Getting link for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        QString strFanPageLink = "";

        OVData::FbFanpage* fbFanPage = getFanPageFromDB( m_strCurrentFanPageId );
        if( !fbFanPage )
        {
            OVFB_MSG( "No fan page found!" );
        }
        else
        {
            if( fbFanPage->link().isNull() || fbFanPage->link().isEmpty() )
            {
                OVFB_WARNING("No fan page link found in database!");
            }
            else
            {
                strFanPageLink = fbFanPage->link();
//                OVFB_MSG("fan page link found: %s", strFanPageLink.toUtf8().constData() );
            }
        }

        return strFanPageLink;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPageCategory()
    {
        OVFB_MSG("Getting category for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        QString strFanPageCategory = "Film";

        OVData::FbFanpage* fbFanPage = getFanPageFromDB( m_strCurrentFanPageId );
        if( !fbFanPage )
        {
            OVFB_MSG( "No fan page found!" );
        }
        else
        {
            if( fbFanPage->category().isNull() || fbFanPage->category().isEmpty() )
            {
                OVFB_WARNING("No fan page category found in database!");
            }
            else
            {
                strFanPageCategory = fbFanPage->category();
//                OVFB_MSG("fan page category found: %s", strFanPageCategory.toUtf8().constData() );
            }
        }

        return strFanPageCategory;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPageStarring()
    {
        OVFB_MSG("Getting 'starring' field for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        QString strFanPageStarring = "";

        OVData::FbFanpage* fbFanPage = getFanPageFromDB( m_strCurrentFanPageId );
        if( !fbFanPage )
        {
            OVFB_MSG( "No fan page found!" );
        }
        else
        {
            if( fbFanPage->starring().isNull() || fbFanPage->starring().isEmpty() )
            {
                OVFB_WARNING("No fan page starring found in database!");
            }
            else
            {
                strFanPageStarring = fbFanPage->starring();
//                OVFB_MSG("fan page starring found: %s", strFanPageStarring.toUtf8().constData() );
            }
        }

        return strFanPageStarring;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPageDirectedBy()
    {
        OVFB_MSG("Getting 'directed by' field for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        QString strFanPageDirectedBy = "";

        OVData::FbFanpage* fbFanPage = getFanPageFromDB( m_strCurrentFanPageId );
        if( !fbFanPage )
        {
            OVFB_MSG( "No fan page found!" );
        }
        else
        {
            if( fbFanPage->directed_by().isNull() || fbFanPage->directed_by().isEmpty() )
            {
                OVFB_WARNING("No fan page directed_by found in database!");
            }
            else
            {
                strFanPageDirectedBy = fbFanPage->directed_by();
//                OVFB_MSG("fan page directed_by found: %s", strFanPageDirectedBy.toUtf8().constData() );
            }
        }

        return strFanPageDirectedBy;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPagePlotOutline()
    {
        OVFB_MSG("Getting 'plot outline' field for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        QString strFanPagePlotOutline = "";

        OVData::FbFanpage* fbFanPage = getFanPageFromDB( m_strCurrentFanPageId );
        if( !fbFanPage )
        {
            OVFB_MSG( "No fan page found!" );
        }
        else
        {
            if( fbFanPage->plot_outline().isNull() || fbFanPage->plot_outline().isEmpty() )
            {
                OVFB_WARNING("No fan page plot_outline found in database!");
            }
            else
            {
                strFanPagePlotOutline = fbFanPage->plot_outline();
//                OVFB_MSG("fan page plot_outline found: %s", strFanPagePlotOutline.toUtf8().constData() );
            }
        }

        return strFanPagePlotOutline;
    }

    int OVAppFacebookDBHandler::getCurrentFanPageFanCount()
    {
        OVFB_MSG("Getting number of fan for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        int iFanPageFanCount = 0;

        OVData::FbFanpage* fbFanPage = getFanPageFromDB( m_strCurrentFanPageId );
        if( !fbFanPage )
        {
            OVFB_MSG( "No fan page found!" );
        }
        else
        {
            if( fbFanPage->fan_count().isNull() || fbFanPage->fan_count().isEmpty() )
            {
                OVFB_WARNING("No fan page fan_count found in database!");
            }
            else
            {
                iFanPageFanCount = fbFanPage->fan_count().toInt();
//                OVFB_MSG("fan page fan_count found: %d", iFanPageFanCount );
            }
        }

        return iFanPageFanCount;
    }

    QStringList OVAppFacebookDBHandler::getListOfFriendsWhoLikedCurrentFanPage()
    {
        OVFB_MSG("Getting list of friends who liked fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        QStringList fansList;

        OVData::FbFanpageFriendList* fbFanPageFriendList = getFanPageFriendListFromDB( m_strCurrentFanPageId );
        if( !fbFanPageFriendList )
        {
            OVFB_MSG( "No fan page friend list found" );
        }
        else
        {
//            OVFB_MSG( "fan page friend list found!" );
            foreach( OVData::FbFriend* fbFriend, fbFanPageFriendList->friends() )
            {
                if( fbFriend->user_id().isNull() || fbFriend->user_id().isEmpty() )
                {
                    OVFB_WARNING("No friend found in database!");
                }
                else
                {
                    fansList.append( fbFriend->user_id() );
//                    OVFB_MSG("friend id found: %s", fbFriend->user_id().toUtf8().constData() );
                }
            }
        }

        return fansList;
    }

    QStringList OVAppFacebookDBHandler::getCurrentFanPagePostsList()
    {
        OVFB_MSG("Getting posts for fan page %s", m_strCurrentFanPageId.toUtf8().constData() );

        QStringList fanPagePostsList;

        OVData::FbFanpagePostList* fbFanPagePostList = getFanPagePostListFromDB( m_strCurrentFanPageId );
        if( !fbFanPagePostList )
        {
            OVFB_MSG( "No fan page post list found!" );
        }
        else
        {
            foreach( OVData::FbFanpagePost* fbFanPagePost, fbFanPagePostList->posts() )
            {
                if( fbFanPagePost->post_id().isNull() || fbFanPagePost->post_id().isEmpty() )
                {
                    OVFB_WARNING("No fan page post found in database!");
                }
                else
                {
                    fanPagePostsList.append( fbFanPagePost->post_id() );
//                    OVFB_MSG("fan page post id found: %s", fbFanPagePost->post_id().toUtf8().constData() );
                }
            }
        }

        return fanPagePostsList;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPagePostUserName( QString _strFanPagePostId )
    {
        OVFB_MSG("Getting user name for fan page post %s", _strFanPagePostId.toUtf8().constData() );

        QString strFanPagePostUserName = "Michael Strode";

        OVData::FbFanpagePost* fbFanPagePost = getFanPagePostFromDB( _strFanPagePostId );
        if( !fbFanPagePost )
        {
            OVFB_MSG( "No fan page post found!" );
        }
        else
        {
            if( getFriendName( fbFanPagePost->user_id() ).isNull() || getFriendName( fbFanPagePost->user_id() ).isEmpty() )
            {
                OVFB_WARNING("No fan page post user name found in database!");
            }
            else
            {
                strFanPagePostUserName = getFriendName( fbFanPagePost->user_id() );
//                OVFB_MSG("fan page post user name found: %s", strFanPagePostUserName.toUtf8().constData() );
            }
        }

        return strFanPagePostUserName;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPagePostUserPicturePath( QString _strFanPagePostId )
    {
        OVFB_MSG("Getting user picture for fan page post %s", _strFanPagePostId.toUtf8().constData() );

        QString strFanPagePostUserPictureFilename = "no_avatar.png";

        OVData::FbFanpagePost* fbFanPagePost = getFanPagePostFromDB( _strFanPagePostId );
        if( !fbFanPagePost )
        {
            OVFB_MSG( "No fan page post found!" );
        }
        else
        {
            if( getFriendPicturePath( fbFanPagePost->user_id() ).isNull() || getFriendPicturePath( fbFanPagePost->user_id() ).isEmpty() )
            {
                OVFB_WARNING("No fan page post user picture found in database!");
            }
            else
            {
                strFanPagePostUserPictureFilename = getFriendPicturePath( fbFanPagePost->user_id() );
//                OVFB_MSG("fan page post user picture found: %s", strFanPagePostUserPictureFilename.toUtf8().constData() );
            }
        }

        return strFanPagePostUserPictureFilename;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPagePostTime( QString _strFanPagePostId )
    {
        OVFB_MSG("Getting time for fan page post %s", _strFanPagePostId.toUtf8().constData() );

        QString strFanPagePostTime = "2010-11-07 19:07:49";

        OVData::FbFanpagePost* fbFanPagePost = getFanPagePostFromDB( _strFanPagePostId );
        if( !fbFanPagePost )
        {
            OVFB_MSG( "No fan page post found!" );
        }
        else
        {
            if( fbFanPagePost->time().toString().isNull() || fbFanPagePost->time().toString().isEmpty() )
            {
                OVFB_WARNING("No fan page post time found in database!");
            }
            else
            {
                strFanPagePostTime = fbFanPagePost->time().toString();
//                OVFB_MSG("fan page post time found: %s", strFanPagePostTime.toUtf8().constData() );
            }
        }

        return strFanPagePostTime;
    }

    QString OVAppFacebookDBHandler::getCurrentFanPagePostMessage( QString _strFanPagePostId )
    {
        OVFB_MSG("Getting message for fan page post %s", _strFanPagePostId.toUtf8().constData() );

        QString strFanPagePostMessage = "Great movie!";

        OVData::FbFanpagePost* fbFanPagePost = getFanPagePostFromDB( _strFanPagePostId );
        if( !fbFanPagePost )
        {
            OVFB_MSG( "No fan page post found!" );
        }
        else
        {
            if( fbFanPagePost->message().isNull() || fbFanPagePost->message().isEmpty() )
            {
                OVFB_WARNING("No fan page post message found in database!");
            }
            else
            {
                strFanPagePostMessage = fbFanPagePost->message();
//                OVFB_MSG("fan page post message found: %s", strFanPagePostMessage.toUtf8().constData() );
            }
        }

        return strFanPagePostMessage;
    }

    bool OVAppFacebookDBHandler::checkContentLike( QString _strUserId, QString _strContentId )
    {
        OVFB_MSG("Checking if user %s is fan of content %s", _strUserId.toUtf8().constData(), _strContentId.toUtf8().constData() );

        bool bIsFan = false;

        OVData::FbContent* fbContent = getContentFromDB( _strContentId );
        if( !fbContent )
        {
            OVFB_ERROR( "Content not found!" );
        }
        else
        {
            foreach( OVData::FbFanpage* fbFanPage, fbContent->fanpageList() )
            {
                OVData::FbFanpageUserList* fbFanPageUserList = getFanPageUserListFromDB( fbFanPage->page_id() );
                if( !fbFanPageUserList )
                {
                    OVFB_MSG( "No fan page user list found" );
                }
                else
                {
                    foreach( OVData::FbUser* fbUser, fbFanPageUserList->users() )
                    {
                        if( fbUser->user_id() == _strUserId )
                        {
                            OVFB_MSG( "User is fan!" );
                            bIsFan = true;
                            break;
                        }
                    }
                }
                if( bIsFan )
                {
                    break;
                }
            }
        }
        return bIsFan;
    }

    bool OVAppFacebookDBHandler::checkFanPageLike( QString _strUserId, QString _strFanPageId )
    {
        OVFB_MSG("Checking if user %s is fan of fan page %s", _strUserId.toUtf8().constData(), _strFanPageId.toUtf8().constData() );

        bool bIsFan = false;

        OVData::FbFanpageUserList* fbFanPageUserList = getFanPageUserListFromDB( _strFanPageId );
        if( !fbFanPageUserList )
        {
            OVFB_MSG( "No fan page user list found" );
        }
        else
        {
//            OVFB_MSG( "fan page user list found!" );
            foreach( OVData::FbUser* fbUser, fbFanPageUserList->users() )
            {
                if( fbUser->user_id() == _strUserId )
                {
                    OVFB_MSG( "User is fan!" );
                    bIsFan = true;
                    break;
                }
            }
        }

        return bIsFan;
    }

    bool OVAppFacebookDBHandler::removeFanPageLike( QString _strUserId, QString _strFanPageId )
    {
        OVFB_MSG("Removing like of fanpage %s for user %s", _strFanPageId.toUtf8().constData(), _strUserId.toUtf8().constData() );

        QString strQueryText( "DELETE FROM FANPAGEUSERLIST WHERE fanpage= (SELECT id FROM FANPAGE WHERE page_id='" + _strFanPageId + "') AND user = (SELECT id FROM USER WHERE user_id='" + _strUserId + "' ) " );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

        if( query.lastError().type() != QSqlError::NoError )
        {
            OVFB_ERROR( "error in query: %s", query.lastError().text().toUtf8().constData() );
            return false;
        }

        return true;
    }






 // DATABASE methods


    OVData::FbUser* OVAppFacebookDBHandler::getUserFromDB( QString _strUserId )
    {
//        OVFB_MSG( "Getting user %s in database...", _strUserId.toUtf8().constData() );

        QString strQueryText( "SELECT id FROM USER WHERE user_id='" + _strUserId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbUser::preparedQueryText );

        if( query.lastError().type() == QSqlError::NoError )
        {
            if( query.next() )
            {
//                return new OVData::FbUser( query.value( 0 ).toInt(), &preparedQuery );
                return new OVData::FbUser( query.value( 0 ).toInt() );
            }
        }
        return NULL;
    }

    bool OVAppFacebookDBHandler::addUserToDB( OVData::FbUser* _newFbUser )
    {
//        OVFB_MSG( "Adding user to database..." );
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbUser )
        {
            OVFB_ERROR( "error while loading FbUser!" );
            return false;
        }

        OVData::FbUser* fbUser = getUserFromDB( _newFbUser->user_id() );
        if( fbUser )
        {
            OVFB_MSG( "User is already in database, updating..." );
            _newFbUser->setId( fbUser->id() );
        }

        if( !_newFbUser->commitData() )
        {
            OVFB_ERROR( "Error while committing user add to database!" );
            return false;
        }

        OVFB_MSG( "User %s successfully added to database!", _newFbUser->user_id().toUtf8().constData() );
        return true;
    }

    OVData::FbUser* OVAppFacebookDBHandler::getUserWithEmailFromDB( QString _strEmail )
    {
        OVFB_MSG( "Getting user with email %s in database...", _strEmail.toUtf8().constData() );

        QString strQueryText( "SELECT id FROM USER WHERE email='" + _strEmail + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbUser::preparedQueryText );

        if( query.lastError().type() == QSqlError::NoError )
        {
            if( query.next() )
            {
//                return new OVData::FbUser( query.value( 0 ).toInt(), &preparedQuery );
                return new OVData::FbUser( query.value( 0 ).toInt() );
            }
        }
        return NULL;
    }


    OVData::FbFriend* OVAppFacebookDBHandler::getFriendFromDB( QString _strFriendId )
    {
//        OVFB_MSG( "Getting friend %s in database...", _strFriendId.toUtf8().constData() );

        QString strQueryText( "SELECT id FROM FRIEND WHERE user_id='" + _strFriendId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbFriend::preparedQueryText );

        if( query.lastError().type() == QSqlError::NoError )
        {
            if( query.next() )
            {
                return new OVData::FbFriend( query.value( 0 ).toInt() );
//                return new OVData::FbFriend( query.value( 0 ).toInt(), &preparedQuery );
            }
        }
        return NULL;
    }

    bool OVAppFacebookDBHandler::addFriendToDB( OVData::FbFriend* _newFbFriend )
    {
//        OVFB_MSG( "Adding friend to database..." );
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbFriend )
        {
            OVFB_ERROR( "error while loading FbFriend!" );
            return false;
        }

        OVData::FbFriend* fbFriend = getFriendFromDB( _newFbFriend->user_id() );
        if( fbFriend )
        {
            OVFB_MSG( "Friend is already in database, updating..." );
            _newFbFriend->setId( fbFriend->id() );
        }

        if( !_newFbFriend->commitData() )
        {
            OVFB_ERROR( "Error while committing friend add to database!" );
            return false;
        }

        OVFB_MSG( "Friend %s successfully added to database!", _newFbFriend->user_id().toUtf8().constData() );
        return true;
    }


    OVData::FbUserFriendList* OVAppFacebookDBHandler::getUserFriendListFromDB( QString _strUserId )
    {
//        OVFB_MSG( "Getting user friend list for user %s in database...", _strUserId.toUtf8().constData() );

        QString strQueryText( "SELECT DISTINCT USERFRIENDLIST.id FROM USER, USERFRIENDLIST WHERE USER.id = USERFRIENDLIST.user AND USER.user_id = '" + _strUserId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbUserFriendList::preparedQueryText );

        OVData::FbUserFriendList* fbUserFriendList = NULL;

        if( query.lastError().type() != QSqlError::NoError )
        {
            OVFB_ERROR( "error in query: %s", query.lastError().text().toUtf8().constData() );
            return NULL;
        }
        else
        {
            if( query.first() )
            {
                fbUserFriendList = new OVData::FbUserFriendList( query.value( 0 ).toInt() );
            }
        }
        return fbUserFriendList;
    }

    bool OVAppFacebookDBHandler::addUserFriendListToDB( OVData::FbUserFriendList* _newFbUserFriendList )
    {
//        OVFB_MSG( "Adding user friend list to database..." );
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbUserFriendList )
        {
            OVFB_ERROR( "error while loading FbUserFriendList!" );
            return false;
        }

        // check if user friend list is already in database
        OVData::FbUserFriendList* fbUserFriendList = getUserFriendListFromDB( _newFbUserFriendList->user()->user_id() );
        if( fbUserFriendList )
        {
            OVFB_MSG( "User friend list is already in database, updating..." );
            _newFbUserFriendList->setId( fbUserFriendList->id() );

            fbUserFriendList->removeFromDB( false );
            // TODO: browse items manually in database table and remove the ones that have to be removed!
        }

        if( !_newFbUserFriendList->commitData() )
        {
            OVFB_ERROR( "Error while committing user friend list add to database!" );
            return false;
        }

        OVFB_MSG( "User friend list successfully added to database!" );
        return true;
    }


    OVData::FbUserfeedpost* OVAppFacebookDBHandler::getUserFeedPostFromDB( QString _strPostId )
    {
//        OVFB_MSG( "Getting user feed post %s in database...", _strPostId.toUtf8().constData() );

        QString strQueryText( "SELECT id FROM USERFEEDPOST WHERE post_id='" + _strPostId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbUserfeedpost::preparedQueryText );

        if( query.lastError().type() == QSqlError::NoError )
        {
            if( query.next() )
            {
                return new OVData::FbUserfeedpost( query.value( 0 ).toInt() );
//                return new OVData::FbUserfeedpost( query.value( 0 ).toInt(), &preparedQuery );
            }
        }
        return NULL;
    }

    bool OVAppFacebookDBHandler::addUserFeedPostToDB( OVData::FbUserfeedpost* _newFbUserFeedPost )
    {
//        OVFB_MSG( "Adding user feed post to database..." );
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbUserFeedPost )
        {
            OVFB_ERROR( "error while loading FbUserfeedpost!" );
            return false;
        }

        // check if user feed post is already in database
        OVData::FbUserfeedpost* fbUserFeedPost = getUserFeedPostFromDB( _newFbUserFeedPost->post_id() );
        if( fbUserFeedPost )
        {
            OVFB_MSG( "User feed post is already in database, updating..." );
            _newFbUserFeedPost->setId( fbUserFeedPost->id() );
        }

        if( !_newFbUserFeedPost->commitData() )
        {
            OVFB_ERROR( "Error while committing user feed post add to database!" );
            return false;
        }

        OVFB_MSG( "User feed post %s successfully added to database!", _newFbUserFeedPost->post_id().toUtf8().constData() );
        return true;
    }


    OVData::FbUserfeedpostList* OVAppFacebookDBHandler::getUserFeedPostListFromDB( QString _strUserId )
    {
//        OVFB_MSG( "Getting user feed post list for user %s in database...", _strUserId.toUtf8().constData() );

        QString strQueryText( "SELECT DISTINCT USERFEEDPOSTLIST.id FROM USER, USERFEEDPOSTLIST WHERE USER.id = USERFEEDPOSTLIST.user AND USER.user_id = '" + _strUserId + "'");

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbUserfeedpostList::preparedQueryText );

        OVData::FbUserfeedpostList* fbUserFeedPostList = NULL;

        if( query.lastError().type() != QSqlError::NoError )
        {
            OVFB_ERROR( "error in query: %s", query.lastError().text().toUtf8().constData() );
            return NULL;
        }
        else
        {
            if( query.first() )
            {
                fbUserFeedPostList = new OVData::FbUserfeedpostList( query.value( 0 ).toInt() );
            }
        }
        return fbUserFeedPostList;
    }

    bool OVAppFacebookDBHandler::addUserFeedPostListToDB( OVData::FbUserfeedpostList* _newFbUserFeedPostList )
    {
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbUserFeedPostList )
        {
            OVFB_ERROR( "error while loading FbUserfeedpostList!" );
            return false;
        }

        // check if user feed post list is already in database
        OVData::FbUserfeedpostList* fbUserFeedPostList = getUserFeedPostListFromDB( _newFbUserFeedPostList->user()->user_id() );
        if( fbUserFeedPostList )
        {
            OVFB_MSG( "User feed post list is already in database, updating..." );
            _newFbUserFeedPostList->setId( fbUserFeedPostList->id() );

            fbUserFeedPostList->removeFromDB( false );
            // TODO: browse items manually in database table and remove the ones that have to be removed!
//            OV_DELETE( fbUserFeedPostList );
        }

        if( !_newFbUserFeedPostList->commitData() )
        {
            OVFB_ERROR( "Error while committing user feed post list add to database!" );
            return false;
        }

        OVFB_MSG( "User feed post list successfully added to database!" );
        return true;
    }


    OVData::FbFanpage* OVAppFacebookDBHandler::getFanPageFromDB( QString _strFanPageId )
    {
//        OVFB_MSG( "Getting fan page %s in database...", _strFanPageId.toUtf8().constData() );

        QString strQueryText( "SELECT id FROM FANPAGE WHERE page_id='" + _strFanPageId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( m_appDatabase );
//        preparedQuery.prepare( OVData::FbFanpage::preparedQueryText );


        if( query.lastError().type() == QSqlError::NoError )
        {
            if( query.next() )
            {
                return new OVData::FbFanpage( query.value( 0 ).toInt() );
//                return new OVData::FbFanpage( query.value( 0 ).toInt(), &preparedQuery );
            }
        }
        return NULL;
    }

    bool OVAppFacebookDBHandler::addFanPageToDB( OVData::FbFanpage* _newFbFanPage )
    {
//        OVFB_MSG( "Adding fan page to database..." );
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbFanPage )
        {
            OVFB_ERROR( "error while loading FbFanpage!" );
            return false;
        }

        // check if fan page is already in database
        OVData::FbFanpage* fbFanPage = getFanPageFromDB( _newFbFanPage->page_id() );
        if( fbFanPage )
        {
            OVFB_MSG( "Fan page is already in database, updating..." );
            _newFbFanPage->setId( fbFanPage->id() );
        }

        if( !_newFbFanPage->commitData() )
        {
            OVFB_ERROR( "Error while committing fan page add to database!" );
            return false;
        }

        OVFB_MSG( "Fan page %s successfully added to database!", _newFbFanPage->page_id().toUtf8().constData() );
        return true;
    }


    OVData::FbFanpagePost* OVAppFacebookDBHandler::getFanPagePostFromDB( QString _strPostId )
    {
//        OVFB_MSG( "Getting fan page post %s in database...", _strPostId.toUtf8().constData() );

        QString strQueryText( "SELECT id FROM FANPAGEPOST WHERE post_id='" + _strPostId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbFanpagePost::preparedQueryText );


        if( query.lastError().type() == QSqlError::NoError )
        {
            if( query.next() )
            {
                return new OVData::FbFanpagePost( query.value( 0 ).toInt() );
//                return new OVData::FbFanpagePost( query.value( 0 ).toInt(), &preparedQuery );
            }
        }
        return NULL;
    }

    bool OVAppFacebookDBHandler::addFanPagePostToDB( OVData::FbFanpagePost* _newFbFanPagePost )
    {
//        OVFB_MSG( "Adding fan page post to database..." );
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbFanPagePost )
        {
            OVFB_ERROR( "error while loading FbFanpagePost!" );
            return false;
        }

        // check if fan page post is already in database
        OVData::FbFanpagePost* fbFanPagePost = getFanPagePostFromDB( _newFbFanPagePost->post_id() );
        if( fbFanPagePost )
        {
            OVFB_MSG( "Fan page post is already in database, updating..." );
            _newFbFanPagePost->setId( fbFanPagePost->id() );
        }

        if( !_newFbFanPagePost->commitData() )
        {
            OVFB_ERROR( "Error while committing fan page post add to database!" );
            return false;
        }

        OVFB_MSG( "Fan page post %s successfully added to database!", _newFbFanPagePost->post_id().toUtf8().constData() );
        return true;
    }


    OVData::FbFanpagePostList* OVAppFacebookDBHandler::getFanPagePostListFromDB( QString _strFanPageId )
    {
//        OVFB_MSG( "Getting fan page post list for fan page %s in database...", _strFanPageId.toUtf8().constData() );

        QString strQueryText( "SELECT DISTINCT FANPAGEPOSTLIST.id FROM FANPAGE, FANPAGEPOSTLIST WHERE FANPAGE.id = FANPAGEPOSTLIST.fanpage AND FANPAGE.page_id = '" + _strFanPageId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbFanpagePostList::preparedQueryText );

        OVData::FbFanpagePostList* fbFanPagePostList = NULL;

        if( query.lastError().type() != QSqlError::NoError )
        {
            OVFB_ERROR( "error in query: %s", query.lastError().text().toUtf8().constData() );
            return NULL;
        }
        else
        {
            if( query.first() )
            {
                fbFanPagePostList = new OVData::FbFanpagePostList( query.value( 0 ).toInt() );
            }
        }
        return fbFanPagePostList;
    }

    bool OVAppFacebookDBHandler::addFanPagePostListToDB( OVData::FbFanpagePostList* _newFbFanPagePostList )
    {
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbFanPagePostList )
        {
            OVFB_ERROR( "error while loading FbFanpagePostList!" );
            return false;
        }

        // check if fan page post list is already in database
        OVData::FbFanpagePostList* fbFanPagePostList = getFanPagePostListFromDB( _newFbFanPagePostList->fanpage()->page_id() );
        if( fbFanPagePostList )
        {
            OVFB_MSG( "Fan page post list is already in database, updating..." );
            _newFbFanPagePostList->setId( fbFanPagePostList->id() );

            fbFanPagePostList->removeFromDB( false );
            // TODO: browse items manually in database table and remove the ones that have to be removed!
        }

        if( !_newFbFanPagePostList->commitData() )
        {
            OVFB_ERROR( "Error while committing fan page post list add to database!" );
            return false;
        }

        OVFB_MSG( "Fan page post list successfully added to database!" );
        return true;
    }


    OVData::FbFanpageUserList* OVAppFacebookDBHandler::getFanPageUserListFromDB( QString _strFanPageId )
    {
//        OVFB_MSG( "Getting fan page user list for fan page %s in database...", _strFanPageId.toUtf8().constData() );

        QString strQueryText( "SELECT DISTINCT FANPAGEUSERLIST.id FROM FANPAGE, FANPAGEUSERLIST WHERE FANPAGE.id = FANPAGEUSERLIST.fanpage AND FANPAGE.page_id = '" + _strFanPageId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbFanpageUserList::preparedQueryText );

        OVData::FbFanpageUserList* fbFanPageUserList = NULL;

        if( query.lastError().type() != QSqlError::NoError )
        {
            OVFB_ERROR( "error in query: %s", query.lastError().text().toUtf8().constData() );
            return NULL;
        }
        else
        {
            if( query.first() )
            {
                fbFanPageUserList = new OVData::FbFanpageUserList( query.value( 0 ).toInt() );
            }
        }
        return fbFanPageUserList;
    }

    bool OVAppFacebookDBHandler::addFanPageUserListToDB( OVData::FbFanpageUserList* _newFbFanPageUserList )
    {
//        OVFB_MSG( "Adding fan page user list to database..." );
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbFanPageUserList )
        {
            OVFB_ERROR( "error while loading FbFanpageUserList!" );
            return false;
        }

        // check if fan page user list is already in database
        OVData::FbFanpageUserList* fbFanPageUserList = getFanPageUserListFromDB( _newFbFanPageUserList->fanpage()->page_id() );
        if( fbFanPageUserList )
        {
            OVFB_MSG( "Fan page user list is already in database, updating..." );
            _newFbFanPageUserList->setId( fbFanPageUserList->id() );

            fbFanPageUserList->removeFromDB( false );
            // TODO: browse items manually in database table and remove the ones that have to be removed!
        }

        if( !_newFbFanPageUserList->commitData() )
        {
            OVFB_ERROR( "Error while committing fan page user list add to database!" );
            return false;
        }

        OVFB_MSG( "Fan page user list successfully added to database!" );
        return true;
    }


    OVData::FbFanpageFriendList* OVAppFacebookDBHandler::getFanPageFriendListFromDB( QString _strFanPageId )
    {
//        OVFB_MSG( "Getting fan page friend list for fan page %s in database...", _strFanPageId.toUtf8().constData() );

        QString strQueryText( "SELECT DISTINCT FANPAGEFRIENDLIST.id FROM FANPAGE, FANPAGEFRIENDLIST WHERE FANPAGE.id = FANPAGEFRIENDLIST.fanpage AND FANPAGE.page_id = '" + _strFanPageId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( *m_appDatabase );
//        preparedQuery.prepare( OVData::FbFanpageFriendList::preparedQueryText );

        OVData::FbFanpageFriendList* fbFanPageFriendList = NULL;

        if( query.lastError().type() != QSqlError::NoError )
        {
            OVFB_ERROR( "error in query: %s", query.lastError().text().toUtf8().constData() );
            return NULL;
        }
        else
        {
            if( query.first() )
            {
                fbFanPageFriendList = new OVData::FbFanpageFriendList( query.value( 0 ).toInt() );
            }
        }
        return fbFanPageFriendList;
    }

    bool OVAppFacebookDBHandler::addFanPageFriendListToDB( OVData::FbFanpageFriendList* _newFbFanPageFriendList )
    {
//        OVFB_MSG( "Adding fan page friend list to database..." );
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbFanPageFriendList )
        {
            OVFB_ERROR( "error while loading FbFanpageFriendList!" );
            return false;
        }

        // check if fan page friend list is already in database
        OVData::FbFanpageFriendList* fbFanPageFriendList = getFanPageFriendListFromDB( _newFbFanPageFriendList->fanpage()->page_id() );
        if( fbFanPageFriendList )
        {
            OVFB_MSG( "Fan page friend list is already in database, updating..." );
            _newFbFanPageFriendList->setId( fbFanPageFriendList->id() );

            fbFanPageFriendList->removeFromDB( false );
            // TODO: browse items manually in database table and remove the ones that have to be removed!
        }

        if( !_newFbFanPageFriendList->commitData() )
        {
            OVFB_ERROR( "Error while committing fan page friend list add to database!" );
            return false;
        }

        OVFB_MSG( "Fan page friend list successfully added to database!" );
        return true;
    }




    bool OVAppFacebookDBHandler::addFriendToFanPageFriendListToDB( OVData::FbFanpage* _fanPage, OVData::FbFriend* _newFbFriend )
    {
//        OVFB_MSG( "Adding friend to fan page friend list in database..." );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_fanPage || !_newFbFriend )
        {
            OVFB_ERROR( "error while loading FbFanpage or FbFriend!" );
            return false;
        }

        // check if friend is already present in fan page friend list
        foreach( OVData::FbFriend *Fbfriend, _fanPage->friendList()->friends() )
        {
            if( Fbfriend->user_id().compare( _newFbFriend->user_id() ) == 0 )
            {
                OVFB_MSG( "Friend is already in fan page friend list" );
                _newFbFriend->setId( Fbfriend->id() );
            }
        }

        // make sure the friend in written in the db
//        _newFbFriend->commitData();       ADD WITH addfriend

        _fanPage->friendList()->setFanpage( _fanPage );
        _fanPage->friendList()->friends().append( _newFbFriend );

        if( !_fanPage->friendList()->commitData() )
        {
            OVFB_ERROR( "error while committing fan page friend list to database!" );
            return false;
        }
        if( !_fanPage->commitData() )
        {
            OVFB_ERROR( "error while committing fan page to database!" );
            return false;
        }

        return true;
    }

    bool OVAppFacebookDBHandler::addUserToFanPageUserListToDB( OVData::FbFanpage* _fanPage, OVData::FbUser* _newFbUser )
    {
//        OVFB_MSG( "Adding user to fan page user list in database..." );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }
        if( !_fanPage )
        {
            OVFB_ERROR( "error while loading FbFanpage!" );
            return false;
        }
        if( !_newFbUser )
        {
            OVFB_ERROR( "error while loading FbUser!" );
            return false;
        }

        // check if user is already present in fan page user list
        foreach( OVData::FbUser *fbUser, _fanPage->userList()->users() )
        {
            if( fbUser->user_id().compare( _newFbUser->user_id() ) == 0 )
            {
                OVFB_MSG( "User is already in fan page user list" );
                _newFbUser->setId( fbUser->id() );
            }
        }

        // make sure the user in written in the db
//        _newFbUser->commitData();       ADD WITH addfriend

        _fanPage->userList()->setFanpage( _fanPage );
        _fanPage->userList()->users().append( _newFbUser );

        if( !_fanPage->userList()->commitData() )
        {
            OVFB_ERROR( "error while committing fan page user list to database!" );
            return false;
        }
        if( !_fanPage->commitData() )
        {
            OVFB_ERROR( "error while committing fan page to database!" );
            return false;
        }

        return true;
    }

    QList<OVData::FbFriend *> OVAppFacebookDBHandler::getFanPageFriendListForUserFromDB( QString _strFanPageId, QString _strUserId )
    {
//        OVFB_MSG( "Getting fan page friend list of user %s for fan page %s in database...", _strUserId.toUtf8().constData(), _strFanPageId.toUtf8().constData() );

        QList<OVData::FbFriend *> fbFriendList;

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
        }
        else
        {
            QString strQueryText( "SELECT DISTINCT FANPAGEFRIENDLIST.friend from FANPAGEFRIENDLIST, FANPAGE, USER, USERFRIENDLIST WHERE FANPAGE.id = FANPAGEFRIENDLIST.fanpage AND FANPAGE.page_id = '" + _strFanPageId + "' AND FANPAGEFRIENDLIST.friend = USERFRIENDLIST.friend AND USER.id = USERFRIENDLIST.user AND USER.user_id = '" + _strUserId + "'" );

            QSqlQuery query = m_appDatabase->exec( strQueryText );

//            QSqlQuery preparedQuery( *m_appDatabase );
//            preparedQuery.prepare( OVData::FbFanpageFriendList::preparedQueryText );

            if( query.lastError().type() != QSqlError::NoError )
            {
                OVFB_ERROR( "error in query: %s", query.lastError().text().toUtf8().constData() );
            }
            else
            {
                while( query.next() )
                {
                    fbFriendList << OV_NEW( Domaine_DATA ) OVData::FbFriend( query.value( 0 ).toInt() );
                }
            }
        }
        return fbFriendList;
    }

    OVData::FbContent* OVAppFacebookDBHandler::getContentFromDB( QString _strContentId )
    {
        OVFB_MSG( "Getting content %s in database...", _strContentId.toUtf8().constData() );

        QString strQueryText( "SELECT id FROM CONTENT WHERE content_id='" + _strContentId + "'" );

        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return NULL;
        }

        QSqlQuery query = m_appDatabase->exec( strQueryText );

//        QSqlQuery preparedQuery( m_appDatabase );
//        preparedQuery.prepare( OVData::FbContent::preparedQueryText );


        if( query.lastError().type() == QSqlError::NoError )
        {
            if( query.next() )
            {
                return new OVData::FbContent( query.value( 0 ).toInt() );
//                return new OVData::FbContent( query.value( 0 ).toInt(), &preparedQuery );
            }
        }
        return NULL;
    }

    bool OVAppFacebookDBHandler::addContentToDB( OVData::FbContent* _newFbContent )
    {
        OVFB_MSG( "Adding content to database..." );
        if( !m_appDatabase )
        {
            OVFB_ERROR( "error: database doesn't exist!" );
            return false;
        }

        if( !_newFbContent )
        {
            OVFB_ERROR( "error while loading FbContent!" );
            return false;
        }

        // check if content is already in database
        OVData::FbContent* fbContent = getContentFromDB( _newFbContent->content_id() );
        if( fbContent )
        {
            OVFB_MSG( "Content is already in database, updating..." );
            _newFbContent->setId( fbContent->id() );
        }

        if( !_newFbContent->commitData() )
        {
            OVFB_ERROR( "Error while committing content add to database!" );
            return false;
        }

        OVFB_MSG( "Content %s successfully added to database!", _newFbContent->content_id().toUtf8().constData() );
        return true;
    }

}
