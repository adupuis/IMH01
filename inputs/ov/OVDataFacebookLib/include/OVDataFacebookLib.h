/*!
    OVDataFacebookLib.h
    Quentin Désert
    Orange Vallée - 2010
*/

#ifndef OVDATAFACEBOOKLIB_H
#define OVDATAFACEBOOKLIB_H

#include <QtCore>
#include <QtNetwork>
#include <QImage>

#include <OVSmartParser.h>


namespace OVData
{

    /*!
        \class FacebookLib
        \brief Abstraction for Facebook
        \details This class is an abstraction for using the Facebook Graph API. Use it for making application that will need to communicate with Facebook
    */
    class FacebookLib : public QObject
    {
        Q_OBJECT

    public:
        /// Constructor
        FacebookLib( QObject* parent = 0 );

        /// Destructor
        ~FacebookLib();

        /// Get the Client ID retrieved from Facebook application (that must have been created on Facebook website)
        /// \return m_strClientId client ID
        QString     getClientId();

        /// Get the Client Secret retrieved from Facebook application (that must have been created on Facebook website)
        /// \return m_strClientSecret client Secret
        QString     getClientSecret();

        /// Get the redirect URI
        /// \return m_strRedirectUri redirect URI
        QString     getRedirectUri();

        /// Get the string containing the list of permissions for the user logged in (separated by ",")
        /// \return m_strPermissionsScope the list of permissions
        QString     getPermissionsScope();

        /// Get the URL for displaying the authorization page. Warning: before calling this method, computeOAuthUrl() must be called
        /// \return m_strOAuthUrl URL for displaying the authorization page
        QString     getOAuthUrl();

        /// Get the id of the logged in user
        /// \return m_strCurrentUserId the current logged in user id
        QString     getCurrentUserId();

        /// Get the URL for getting the access token after user authorized the application
        /// \return m_strGetAccessTokenUrl the access token URL
        QString     getGetAccessTokenUrl();

        /// Get the current access token obtained after user authorized the application
        /// \return m_strCurrentAccessToken the current access token
        QString     getCurrentAccessToken();

        /// Set the current access token obtained after user authorized the application
        /// \param _strAccessToken access token obtained after user authorized the application
        void        setCurrentAccessToken( const QString& _strAccessToken );

        /// Get the friends list (m_friendsList) found with method retrieveFriendsList()
        /// \return m_friendsList friends list found
        QMap< int, QMap<QString, QString> > getFriendsList();

        /// Get the fan pages IDs (m_fanPagesIdsFound) found with method searchFanPage()
        /// \return m_fanPagesIdsFound list of fan page ids found
        QStringList getFanPagesIdsFound();

        /// Get the user ID (m_strUserIdFound) found with method retrieveUserId()
        /// \return m_strUserIdFound user ID
        QString     getUserIdFound();

        /// Get the user name (m_strUserNameFound) found with method retrieveUserName()
        /// \return m_strUserNameFound user name
        QString     getUserNameFound();

        /// Get the user picture (m_userPicture) found with method retrieveUserPicture()
        /// \return m_userPicture user picture
        QImage      getUserPictureFound();

        /// Get the friend picture (m_friendPicture) found with method retrieveFriendPicture()
        /// \return m_friendPicture friend picture
        QImage      getFriendPictureFound();

        /// Get the fan pages IDs (m_userLikesFound) found with method retrieveUserLikes()
        /// \return m_userLikesFound list of fan page ids found
        QStringList getUserLikesFound();

        /// Get the user feed post list (m_userFeedPostListFound) found with method retrieveUserFeedPostList()
        /// \return m_userFeedPostListFound user feed post list
        QMap< int, QMap<QString, QString> > getUserFeedPostListFound();

        /// Get the user feed poster name (m_strUserFeedPosterNameFound) found with method retrieveUserFeedPosterName()
        /// \return m_strUserFeedPosterNameFound user feed poster name
        QString     getUserFeedPosterNameFound();

        /// Get the user feed poster picture (m_UserFeedPosterPictureFound) found with method retrieveUserFeedPosterPicture()
        /// \return m_UserFeedPosterPictureFound user feed poster picture
        QImage      getUserFeedPosterPictureFound();

        /// Get the fan page information (m_fanPageInformation) found with method retrieveFanPageInformation()
        /// \return m_fanPageInformation map with fan page information
        QMap<QString, QString> getFanPageInformationFound();

        /// Get the fan page picture (m_fanPagePicture) found with method retrieveFanPagePicture()
        /// \return m_fanPagePicture fan page picture
        QImage      getFanPagePictureFound();

        /// Get the post list of a fan page (m_FanPagePostListFound) found with method retrieveFanPagePostList()
        /// \return m_FanPagePostListFound post list of a fan page
        QMap< int, QMap<QString, QString> > getFanPagePostListFound();

        /// Get the fan page poster name (m_strFanPagePosterNameFound) found with method retrieveFanPagePosterName()
        /// \return m_strFanPagePosterNameFound fan page poster name
        QString     getFanPagePosterNameFound();

        /// Get the fan page poster picture (m_FanPagePosterPictureFound) found with method retrieveFanPagePosterPicture()
        /// \return m_FanPagePosterPictureFound fan page poster picture
        QImage      getFanPagePosterPictureFound();

        /// Get the post ID just posted (m_strPostId) found with method postMessage()
        /// \return m_strPostId post ID (just posted)
        QString     getPostId();

        /// Search a fan page for a specific content. Save fan pages IDs found in m_fanPagesIdsFound (QStringList)
        /// \param _strContent content used for the search
        void        searchFanPage( QString& _strContent );

        /// Get ID of a specific user using current access_token. Save ID found in m_strUserIdFound (QString)
        void        retrieveUserId();

        /// Get the name of a specific user. Save name found in m_strUserNameFound (QString)
        /// \param _strUserId user's ID
        void        retrieveUserName( const QString& _strUserId );

        /// Get the profile picture of a specific user. Save profile picture found in m_userPicture (QImage)
        /// \param _strUserId user's ID
        void        retrieveUserPicture( const QString& _strUserId );

        /// Retrieve friends list of a specific user, using the access_token associated with the account. Save friends list found in m_friendsListFound (QStringList)
        /// \param _strUserId user's ID
        void        retrieveFriendsList( QString& _strUserId );

        /// Get the profile picture of a specific friend. Save profile picture found in m_friendPicture (QImage)
        /// \param _strFriendId friends's ID
        void        retrieveFriendPicture( const QString& _strFriendId );

        /// Get the likes of a specific user. Save likes found in m_userLikes (QStringList)
        /// \param _strUserId user's ID
        void        retrieveUserLikes( const QString& _strUserId );

        /// Retrieve news feed of a specific user, using the access_token associated with the account. Save news feed found in m_userFeedPostListFound (QMap)
        /// \param _strUserId user's ID
        void        retrieveUserFeedPostList( QString& _strUserId );

        /// Get the name of a specific user feed poster. Save name found in m_strUserFeedPosterNameFound (QString)
        /// \param _strUserFeedPosterId user feed poster's ID
        void        retrieveUserFeedPosterName( const QString& _strUserFeedPosterId );

        /// Get the profile picture of a specific user feed poster. Save profile picture found in m_UserFeedPosterPictureFound (QImage)
        /// \param _strUserFeedPosterId user feed poster's ID
        void        retrieveUserFeedPosterPicture( const QString& _strUserFeedPosterId );

        /// Retrieve information of a specific fan page. Save information found in m_fanPageInformation (QMap)
        /// \param _strFanPageId Facebook ID of the fan page
        void        retrieveFanPageInformation( QString& _strFanPageId );

        /// Retrieve the profile picture of a specific fan page. Save profile picture found in m_fanPagePicture (QImage)
        /// \param _strFanPageId Facebook ID of the fan page
        void        retrieveFanPagePicture( QString& _strFanPageId );

        /// Retrieve the post list of a specific fan page. Save post list found in m_FanPagePostListFound (QMap)
        /// \param _strFanPageId Facebook ID of the fan page
        void        retrieveFanPagePostList( QString& _strFanPageId );

        /// Get the name of a specific fan page poster. Save name found in m_strFanPagePosterNameFound (QString)
        /// \param _strFanPagePosterId fan page poster's ID
        void        retrieveFanPagePosterName( const QString& _strFanPagePosterId );

        /// Get the profile picture of a specific fan page poster. Save profile picture found in m_fanPagePosterPictureFound (QImage)
        /// \param _strFanPagePosterId fan page poster's ID
        void        retrieveFanPagePosterPicture( const QString& _strFanPagePosterId );

        /// Post a message on the wall of a specific user. Save post id (just posted) in m_strPostId (QString)
        /// \param _strUserId user's ID
        /// \param _strLink link of the fanpage associated with the message
        /// \param _strMessage message to post
        void        postMessage( const QString& _strUserId, QString& _strLink, QString& _strMessage );


    private:

        /// Modify SSL configuration of a request (for avoiding SSL handshake failure)
        QNetworkRequest     setSSLConfig();

        void                startRequest( QString& _strUrl );

        /// Convert date from non-readable API date to human-readable date
        QString             convertDate( QString& _strApiDate );

        QString             replaceSpecialChars( QString& _strString );

        QString             escapeSpecialChars( QString& _strString );

        QString             removePercentEncoding( QString& _strString );

    signals:

        void        sigRequestFailed();

        /// Signal emitted when a friends list Request is finished
        void        sigFriendsListRequestFinished();

        /// Signal emitted when a Fan Page Request is finished
        void        sigFanPageRequestFinished();

        /// Signal emitted when a User ID Request is finished
        void        sigUserIdRequestFinished();

        /// Signal emitted when a User name Request is finished
        void        sigUserNameRequestFinished();

        /// Signal emitted when a User Picture Request is finished
        void        sigUserPictureRequestFinished();


        /// Signal emitted when a Friend Picture Request is finished
        void        sigFriendPictureRequestFinished();

        /// Signal emitted when a User likes Request is finished
        void        sigUserLikesRequestFinished();
        void        sigUserLikesRequestFailed();


        /// Signal emitted when a User News feed Request is finished
        void        sigUserFeedPostListRequestFinished();

        /// Signal emitted when a user feed Poster Name Request is finished
        void        sigUserFeedPosterNameRequestFinished();

        /// Signal emitted when a user feed Poster Picture Request is finished
        void        sigUserFeedPosterPictureRequestFinished();


        /// Signal emitted when a Fan page information Request is finished
        void        sigFanPageInformationRequestFinished();

        /// Signal emitted when a Fan page Picture Request is finished
        void        sigFanPagePictureRequestFinished();

        /// Signal emitted when a Fan page post list Request is finished
        void        sigFanPagePostListRequestFinished();

        /// Signal emitted when a fan page Poster Name Request is finished
        void        sigFanPagePosterNameRequestFinished();

        /// Signal emitted when a fan page Poster Picture Request is finished
        void        sigFanPagePosterPictureRequestFinished();


        /// Signal emitted when a Post Message Request is finished
        void        sigPostMessageRequestFinished();


    private slots:

    //    void        checkNetworkReplyProgress( qint64 _bytesReceived, qint64 _bytesTotal );

        /// Handle data returned by the request made in the searchFanPage method
        /// \param _networkReply object containing the downloaded data as well as meta-data (headers, etc.)
    //    void        slotHandleNetworkData( QNetworkReply* _networkReply );
        void        slotHandleNetworkData();

    //    void        slotHandleNetworkError( QNetworkReply::NetworkError );


    private:

    //    QMutex                      m_mutex;

        /// OVSmartParser object for using config file
        OVSmartParser*              m_parser;

        /// Client ID retrieved from Facebook application (that must have been created on Facebook website)
        QString                     m_strClientId;

        /// Client Secret retrieved from Facebook application (that must have been created on Facebook website)
        QString                     m_strClientSecret;

        /// Redirect URI used for getting access_token
        QString                     m_strRedirectUri;

        /// String containing the list of permissions for the user logged in (separated by ",")
        QString                     m_strPermissionsScope;

        /// Base URL for displaying the authorization page. Strings between "<>" must be replaced in calling computeOAuthUrl()
        QString                     m_strOAuthBaseUrl;

        /// URL for displaying the authorization page
        QString                     m_strOAuthUrl;

        /// User ID of the user currently logged in
        QString                     m_strCurrentUserId;

        /// Base URL for getting the access token after user authorized the application. Strings between "<>" must be replaced in calling computeGetAccessTokenUrl()
        QString                     m_strGetAccessTokenBaseUrl;

        /// URL for getting the access token after user authorized the application
        QString                     m_strGetAccessTokenUrl;

        /// Access token associated with the user currently logged in
        QString                     m_strCurrentAccessToken;

        /// Tag indicating which action to execute when receiving data from network request
        QString                     m_strHandleDataActionTag;

        /// URL for getting friends list
        QString                     m_strFriendsListUrl;

        /// URL for searching fan pages
        QString                     m_strSearchUrl;

    //    QTimer                      m_downloadTimer;

        /// Network Access Manager for connecting to Facebook API
        QNetworkAccessManager*      m_networkAccessManager;

        QNetworkReply*              m_networkReply;

        int                         m_iNbDownloadAttempts;

        QString                     m_strUrl;

        /// List of friends of a specific user found with method retrieveFriendsList()
        QStringList                 m_friendsListFound;

        /// Map containing friends list of a specific user found with method retrieveFriendsList(). Each entry contains the name and the ID of the user.
        QMap< int, QMap<QString, QString> > m_friendsList;

        /// Content to search
        QString                     m_strContent;

        /// List of IDs of fan pages found with method searchFanPage()
        QStringList                 m_fanPagesIdsFound;

        /// URL for getting user ID
        QString                     m_strUserIdUrl;

        /// User ID returned by request
        QString                     m_strUserIdFound;

        /// URL for getting user name
        QString                     m_strUserNameUrl;

        /// User Name returned by request
        QString                     m_strUserNameFound;

        /// URL for getting user picture
        QString                     m_strUserPictureUrl;

        /// User Picture returned by request
        QImage                      m_userPictureFound;

        /// Friend Picture returned by request
        QImage                      m_friendPictureFound;

        /// URL for getting user likes
        QString                     m_strUserLikesUrl;

        /// List of IDs of fan pages found with method retrieveUserLikes()
        QStringList                 m_userLikesFound;

        /// URL for getting news feed of a user
        QString                     m_strUserFeedPostListUrl;

        /// News feed of a user returned by request
        QMap< int, QMap<QString, QString> > m_userFeedPostListFound;

        /// User feed Poster Name returned by request
        QString                     m_strUserFeedPosterNameFound;

        /// User feed Poster Picture returned by request
        QImage                      m_UserFeedPosterPictureFound;

        /// URL for getting fan page information
        QString                     m_strFanPageUrl;

        /// Fan page information returned by request
        QMap<QString, QString>      m_fanPageInformation;

        /// Fan page Picture returned by request
        QImage                      m_fanPagePicture;

        /// URL for getting fan page post list
        QString                     m_strFanPagePostListUrl;

        /// Fan page post list returned by request
        QMap< int, QMap<QString, QString> > m_FanPagePostListFound;

        /// Fan page Poster Name returned by request
        QString                     m_strFanPagePosterNameFound;

        /// Fan page Poster Picture returned by request
        QImage                      m_fanPagePosterPictureFound;

        /// URL for posting a message on a user's wall
        QString                     m_strPostMessageUrl;

        /// Post ID (just posted) returned by request
        QString                     m_strPostId;

        /// Number of fan pages to save when searching fan pages with Facebook search API
        int                         m_iNbPagesToSave;

        /// Number of redirections of an URL
        int                         m_iNbRedirections;

        /// Maximum number of redirections allowed for an URL
        int                         m_iNbMaxRedirections;
    };

}

#endif
