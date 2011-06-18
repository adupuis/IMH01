#ifndef OVAPPFACEBOOKJOB_H
#define OVAPPFACEBOOKJOB_H

#include <QSqlDatabase>


#include <OVDataEpgLib>
#include <OVDataFacebookGen.h>
#include <OVDataFacebookLib.h>

#include <OVSmartParser.h>

#include <OVSchematicsAppPluginJob.h>

#include <OVAppFacebookDBHandler.h>
#include <OVAppFacebookInterface.h>


namespace OVSchematics
{
        class AppFacebookJob : public AppPluginJob
        {
            Q_OBJECT

        public:

            AppFacebookJob();

            ~AppFacebookJob();

            void                    connectToDatabase();

            void                    setInterface( OVAppFacebookInterface* _fbInterface );


        private:

            virtual void    work();

            virtual void    slotMessageFromInterfaceReceived( int _iID, QVariant _message );

            virtual void    getMessageFromUI(int _iID, QVariant _messageValue);


            bool            isContentInDB( QString _strContentId );

            void            checkIfCurrentUserLikesContent( QString _strContentId );

            void            getLikesForContentOverNetwork();

            void            stopLikesTimer();

            void            getLikesForContent();

            void            shareMessage( QString _strContent, QString _strMessage );

            void            makeLikeOnContent();

            void            makeUnlikeOnContent();


            void            setJobTerminated( int _iMessageForInterface, QString _strJobName );

            void            processJobsQueue();

            void            sendSyncStatus( bool _bIsSyncRunning );


            void            startRequestsForCategoryUserNewsFeed();

            void            startRequestsForCategoryFriendList();

            void            startRequestsForCategoryFanPage();


            void            requestUserId();

            void            requestUserName( QString _strUserId );

            void            requestUserPicture( QString _strUserId );

            void            requestUserFeedPostList( QString _strUserId );

            void            requestUserFeedPostersName();

            void            requestUserFeedPostersPicture();


            void            requestFriendsList( QString _strUserId );

            void            requestFriendsPicture();

            void            requestFriendsLikes();

            void            requestUserLikes( QString _strUserId );


            void            requestFanPageInformation( QString _strFanPageId );

            void            requestFanPagePicture( QString _strFanPageId );

            void            requestFanPagePostList( QString _strFanPageId );

            void            requestFanPagePostersName();

            void            requestFanPagePostersPicture();


            void            requestSearchFanPage( QString _strContent );


            void            requestShareMessage( QString _strUserId, QString _strContent, QString _strMessage );


            QDateTime       convertFbDateTimeToQDateTime( QString _strFbDateTime );


        signals:


        private slots:

            void slotGetLikes();


            void slotHandleFailedRequest();


            void slotHandleUserIdRequest();

            void slotHandleUserNameRequest();

            void slotHandleUserPictureRequest();

            void slotHandleUserFeedPostListRequest();

            void slotHandleUserFeedPostersNameRequest();

            void slotHandleUserFeedPostersPictureRequest();


            void slotHandleFriendsListRequest();

            void slotHandleFriendsPictureRequest();

            void slotHandleUserLikesRequest();
            void slotHandleUserLikesFailed();


            void slotHandleFanPageInformationRequest();

            void slotHandleFanPagePictureRequest();

            void slotHandleFanPagePostListRequest();

            void slotHandleFanPagePostersNameRequest();

            void slotHandleFanPagePostersPictureRequest();


            void slotHandleSearchFanPageRequest();


            void slotHandleShareMessageRequest();


        private:

//            QMutex                          m_mutex;

            /// OVSmartParser object for using config file
            OVSmartParser*                  m_parser;

            OVData::FacebookLib*            m_facebookLib;

            QSqlDatabase*                   m_appDatabase;

            OVAppFacebookDBHandler*         m_dbHandler;

            OVAppFacebookInterface*         m_FBInterface;

            OVData::FbUser*                 m_fbUser;

            OVData::FbUserFriendList*       m_fbFriendList;

            OVData::FbFanpageFriendList*    m_fbFanPageFriendList;

            OVData::FbUserfeedpostList*     m_fbUserFeedPostList;

            OVData::FbFanpage*              m_fbFanPage;

            OVData::FbFanpagePostList*      m_fbFanpagePostList;

            QList<OVData::FbFanpage*>       m_fbFanPageQList;

            bool                            m_bIsFacebookSyncInProgress;

            bool                            m_bJobRunning;

            bool                            m_bUpdateModeOn;

            bool                            m_bAuthInProgress;

            bool                            m_bCurrentUserLikeSearch;

            QString                         m_strCurrentJob;

            QString                         m_strHandleFailedRequestTag;

            QString                         m_strCurrentUserId;

            QString                         m_strCurrentUserEmail;

            QString                         m_strCurrentUserPwd;

            QString                         m_strCurrentAccessToken;

            QString                         m_strCurrentFanPageId;

            QString                         m_strContentId;

            QString                         m_strContentTitle;

            QString                         m_strContentType;

            QString                         m_strContentToShare;

            QString                         m_strMessageToShare;

            QString                         m_strPicturesFilepath;

            QStringList                     m_jobsQueue;

            QStringList                     m_userFeedPosters;

            QStringList                     m_fanPagePosters;

            QStringList                     m_fanPagesFound;

            QStringList                     m_fanPagesToUnlike;

            SocialMessage                   m_socialMessage;

            int                             m_iWorkCounter;

            int                             m_iFriendsJobCounter;

            int                             m_iFriendNumber;

            int                             m_iNbFriends;

            int                             m_iNbUserFeedPosters;

            int                             m_iNbFanPagePosters;

            int                             m_iFanPageNumber;

            int                             m_iNbFanPagesFound;

            int                             m_iNbLikesRequestsInProgress;

            QMap<int, QString>              m_fanPagesMap;

            QTimer                          m_likesTimer;
        };
}

#endif // OVAPPFACEBOOKJOB_H
