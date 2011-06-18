#ifndef OVAPPFACEBOOKDBHANDLER_H
#define OVAPPFACEBOOKDBHANDLER_H

#include <QtCore>
#include <QSqlDatabase>


#include <OVDataEpgLib>
#include <OVDataFacebookGen.h>


namespace OVSchematics
{
    class OVAppFacebookDBHandler
    {

    public:

        OVAppFacebookDBHandler();

        ~OVAppFacebookDBHandler();


        void        setAppDatabase( QSqlDatabase* _appDatabase );


        /// Get filepath for pictures
        QString     getPicturesFilepath();

        /// Set filepath for pictures
        void        setPicturesFilepath( QString _strFilepath );

        /// Get the logged in user id (m_strUserId)
        QString     getCurrentUserId();

        /// Set the logged in user id (m_strUserId)
        void        setCurrentUserId( QString _strUserId );

        /// Get the fan page id to display (m_strFanPageId)
        QString     getCurrentFanPageId();

        /// Set the fan page id to display (m_strFanPageId)
        void        setCurrentFanPageId( QString _strFanPageId );





        /// Get the logged in user name
        QString     getCurrentUserName();

        /// Get the logged in user picture
        QString     getCurrentUserPicturePath();

        /// Get the logged in user friend list
        /// \return list of user facebook ids
        QStringList getCurrentUserFriendList();

        /// Get a friend user name from its user id
        QString     getFriendName( QString _strFriendUserId );

        /// Get a friend user picture from its user id
        QString     getFriendPicturePath( QString _strFriendUserId );

        /// Get the logged in user feed posts list
        /// \return list of ids of user feed posts
        QStringList getCurrentUserFeedPostsList();

        /// Get the name of the post's user from the post id
        QString     getCurrentUserFeedPostUserName( QString _strUserFeedPostId );

        /// Get the picture of the post's user from the post id
        QString     getCurrentUserFeedPostUserPicturePath( QString _strUserFeedPostId );

        /// Get the time of the post from the post id
        QString     getCurrentUserFeedPostTime( QString _strUserFeedPostId );

        /// Get the message of the post from the post id
        QString     getCurrentUserFeedPostMessage( QString _strUserFeedPostId );

        /// Get the name of the current fan page
        QString     getCurrentFanPageName();

        /// Get the picture of the current fan page
        QString     getCurrentFanPagePicturePath();

        /// Get the link of the current fan page
        QString     getCurrentFanPageLink();

        /// Get the category of the current fan page
        QString     getCurrentFanPageCategory();

        /// Get the 'starring' field of the current fan page
        QString     getCurrentFanPageStarring();

        /// Get the 'directed by' field of the current fan page
        QString     getCurrentFanPageDirectedBy();

        /// Get the 'plot outline' field of the current fan page
        QString     getCurrentFanPagePlotOutline();

        /// Get the number of fans of the current fan page
        int         getCurrentFanPageFanCount();

        /// Get the list of friends who liked the current fan page
        /// \return list of user facebook ids
        QStringList getListOfFriendsWhoLikedCurrentFanPage();

        /// Get the current fan page posts list
        /// \return list of ids of fan page posts
        QStringList getCurrentFanPagePostsList();

        /// Get the name of the fan page post's user from the post id
        QString     getCurrentFanPagePostUserName( QString _strFanPagePostId );

        /// Get the picture of the fan page post's user from the post id
        QString     getCurrentFanPagePostUserPicturePath( QString _strFanPagePostId );

        /// Get the time of the fan page post from the post id
        QString     getCurrentFanPagePostTime( QString _strFanPagePostId );

        /// Get the message of the fan page post from the post id
        QString     getCurrentFanPagePostMessage( QString _strFanPagePostId );


        bool        checkContentLike( QString _strUserId, QString _strContentId );
        bool        checkFanPageLike( QString _strUserId, QString _strFanPageId );

        bool        removeFanPageLike( QString _strUserId, QString _strFanPageId );





        OVData::FbUser*                     getUserFromDB( QString _strUserId );
        bool                                addUserToDB( OVData::FbUser* _newFbUser );

        OVData::FbUser*                     getUserWithEmailFromDB( QString _strEmail );

        OVData::FbFriend*                   getFriendFromDB( QString _strFriendId );
        bool                                addFriendToDB( OVData::FbFriend* _newFbFriend );

        OVData::FbUserFriendList*           getUserFriendListFromDB( QString _strUserId );
        bool                                addUserFriendListToDB( OVData::FbUserFriendList* _newFbUserFriendList );

        OVData::FbUserfeedpost*             getUserFeedPostFromDB( QString _strPostId );
        bool                                addUserFeedPostToDB( OVData::FbUserfeedpost* _newFbUserFeedPost );

        OVData::FbUserfeedpostList*         getUserFeedPostListFromDB( QString _strUserId );
        bool                                addUserFeedPostListToDB( OVData::FbUserfeedpostList* _newFbUserFeedPostList );

        OVData::FbFanpage*                  getFanPageFromDB( QString _strFanPageId );
        bool                                addFanPageToDB( OVData::FbFanpage* _newFbFanPage );

        OVData::FbFanpagePost*              getFanPagePostFromDB( QString _strPostId );
        bool                                addFanPagePostToDB( OVData::FbFanpagePost* _newFbFanPagePost );

        OVData::FbFanpagePostList*          getFanPagePostListFromDB( QString _strFanPageId );
        bool                                addFanPagePostListToDB( OVData::FbFanpagePostList* _newFbFanPagePostList );

        OVData::FbFanpageUserList*          getFanPageUserListFromDB( QString _strFanPageId );
        bool                                addFanPageUserListToDB( OVData::FbFanpageUserList* _newFbFanPageUserList );

        OVData::FbFanpageFriendList*        getFanPageFriendListFromDB( QString _strFanPageId );
        bool                                addFanPageFriendListToDB( OVData::FbFanpageFriendList* _newFbFanPageFriendList );

        bool                                addFriendToFanPageFriendListToDB( OVData::FbFanpage* _fanPage, OVData::FbFriend* _newFbFriend );
        bool                                addUserToFanPageUserListToDB( OVData::FbFanpage* _fanPage, OVData::FbUser* _newFbUser );

        QList<OVData::FbFriend *>           getFanPageFriendListForUserFromDB( QString _strFanPageId, QString _strUserId );

        OVData::FbContent*                  getContentFromDB( QString _strContentId );
        bool                                addContentToDB( OVData::FbContent* _newFbContent );





    private:

        /// File path of pictures
        QString             m_strPicturesFilepath;

        /// ID of user logged id
        QString             m_strCurrentUserId;

        /// ID of current fan page to display
        QString             m_strCurrentFanPageId;

        QSqlDatabase*       m_appDatabase;
    };
}

#endif // OVAPPFACEBOOKDBHANDLER_H
