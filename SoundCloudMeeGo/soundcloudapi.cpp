#include "soundcloudapi.h"

SoundCloudApi::SoundCloudApi( )
    : urlRoot ( "https://api.soundcloud.com" )
    , urlUsers ( "/users" )
    , urlTracks ( "/tracks" )
    , urlPlaylists( "/playlists" )
    , urlFollowings ( "/followings" )
    , urlFollowers ( "/followers" )
    , urlGroups( "/groups" )
    , urlComments( "/comments" )
    , urlFavorites( "/favorites" )
    , urlFavoriters( "/favoriters" )
    , urlPermissions( "/permissions" )
    , urlSecretToken( "/secret-token" )
    , urlModerators( "/moderators")
    , urlMembers( "/members" )
    , urlContributors( "/contributors" )
    , urlPendingTracks( "/pending_tracks" )
    , urlContributions( "/contributions" )
    , urlMe( "/me" )
    , urlConnections( "/connections" )
    , urlActivities( "/activities" )
    , urlApps( "/apps" )
    , urlResolve( "/resolve" )
    , mReplyType( -1 )
{
    mNetworkAccessManager = new QNetworkAccessManager;
    mParser = new QJson::Parser;
}

SoundCloudApi::~SoundCloudApi()
{
    delete mNetworkAccessManager;
}

void SoundCloudApi::setOAuthToken( QString & _OAuthToken )
{
    mOAuthToken = _OAuthToken;
}

// Users
void SoundCloudApi::getUsers( )
{
    QString url = urlRoot;
    url.append( urlUsers );

    startRequest( url );
}

void SoundCloudApi::getUser( int user_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );

    startRequest( url );
}

void SoundCloudApi::getTrackForUser( int user_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlTracks );

    startRequest( url );
}

void SoundCloudApi::getPlaylistForUser( int user_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlPlaylists );


    startRequest( url );
}

void SoundCloudApi::getFollowingsForUser( int user_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFollowings );


    startRequest( url );
}

void SoundCloudApi::getFollowingForUser( int user_id, int following_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFollowings );
    url.append( "/" );
    url.append( following_id );


    startRequest( url );
}

void SoundCloudApi::addFollowingForUser( int user_id, User * following )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFollowings );
    url.append( "/" );
    url.append( following->mId );


    //startRequest( url );
}

void SoundCloudApi::deleteFollowingForUser( int user_id, User * following )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFollowings );
    url.append( "/" );
    url.append( following->mId );


    //startRequest( url );
}

void SoundCloudApi::getFollowersForUser( int user_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFollowers );


    startRequest( url );
}

void  SoundCloudApi::getFollowerForUser( int user_id, int follower_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFollowers );
    url.append( "/" );
    url.append( follower_id );


    //startRequest( url );
}

void SoundCloudApi::getCommentsForUser( int user_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlComments );


    startRequest( url );
}

void SoundCloudApi::getFavoritesForUser( int user_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFavorites );


    startRequest( url );
}

void SoundCloudApi::getFavoriteForUser( int user_id, int favorite_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFavorites );
    url.append( "/" );
    url.append( favorite_id );


    startRequest( url );
}

void SoundCloudApi::addFavoriteForUser( int user_id, Track * favorite )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFavorites );
    url.append( "/" );
    url.append( favorite->mId );


    // POST
    //startRequest( url );
}

void SoundCloudApi::deleteFavoriteForUser( int user_id, Track * favorite )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlFavorites );
    url.append( "/" );
    url.append( favorite->mId );


    // DELETE
    //startRequest( url );
}

void SoundCloudApi::getGroupsForUser( int user_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( user_id );
    url.append( urlGroups );


    startRequest( url );
}

// Tracks
void SoundCloudApi::getTracks( )
{
    QString url = urlRoot;
    url.append( urlTracks );


    startRequest( url );
}

void SoundCloudApi::getTrack( int track_id )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( QString::number(track_id) );

    mReplyType = TRACK;

    startRequest( url );
}

void SoundCloudApi::addTrack( Track * track )
{
    QString url = urlRoot;
    url.append( urlTracks );


    // POST
    //startRequest( url );
}

void SoundCloudApi::deleteTrack( Track * track )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( track->mId );


    // DELETE
    //startRequest( url );
}

void SoundCloudApi::getCommentsForTrack( int track_id )
{
    QString url = urlRoot;
    url.append( urlUsers );
    url.append( "/" );
    url.append( track_id );
    url.append( urlComments );


    startRequest( url );
}

void SoundCloudApi::addCommentForTrack( int track_id, Comment * comment )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( track_id );
    url.append( urlComments );


    // POST
    //startRequest( url );
}

void SoundCloudApi::deleteCommentForTrack( int track_id, Comment * comment )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( track_id );
    url.append( urlComments );
    url.append( "/" );
    url.append( comment->mId );



    // DELETE
    //startRequest( url );
}

void SoundCloudApi::getCommentForTrack( int track_id, int comment_id )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( track_id );
    url.append( urlComments );
    url.append( "/" );
    url.append( comment_id );



    startRequest( url );
}

void SoundCloudApi::getFavoritersForTrack( int track_id )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( track_id );
    url.append( urlFavoriters );



    startRequest( url );
}

void SoundCloudApi::getFavoriterForTrack( int track_id, int favoriter_id )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( track_id );
    url.append( urlFavoriters );
    url.append( "/" );
    url.append( favoriter_id );



    startRequest( url );
}

// TODO: Vérifier le type de retour
void SoundCloudApi::getPermissionsForTrack( int track_id )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( track_id );
    url.append( urlPermissions );



    startRequest( url );
}

// TODO: Vérifier le type de retour
void SoundCloudApi::getPermissionForTrack( int track_id, int permission_id )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( track_id );
    url.append( urlPermissions );
    url.append( "/" );
    url.append( permission_id );



    startRequest( url );
}

void SoundCloudApi::addPermissionForTrack( int user_id, User * permission )
{

}

void SoundCloudApi::deletePermissionForTrack( int user_id, User * permission )
{

}

void SoundCloudApi::getSecretTokenForTrack( int track_id )
{
    QString url = urlRoot;
    url.append( urlTracks );
    url.append( "/" );
    url.append( track_id );
    url.append( urlSecretToken );



    startRequest( url );
}

void SoundCloudApi::setSecretTokenForTrack( int track_id, QString * secretToken )
{

}


// Playlists
void SoundCloudApi::getPlaylists( )
{
    QString url = urlRoot;
    url.append( urlPlaylists );



    startRequest( url );
}

void SoundCloudApi::getPermissionsForPlaylist( int playlist_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( playlist_id );



    startRequest( url );
}

void SoundCloudApi::getPermissionForPlaylist( int playlist_id, int permission_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( playlist_id );
    url.append( urlPermissions );


    startRequest( url );
}

void SoundCloudApi::addPermissionForPlaylist( int playlist_id, User * permission )
{

}

void SoundCloudApi::deletePermissionForPlaylist( int playlist_id, User * permission )
{

}

void SoundCloudApi::getSecretTokenForPlaylist( int playlist_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( playlist_id );
    url.append( urlSecretToken );


    startRequest( url );
}

void SoundCloudApi::setSecretTokenForPlaylist( int playlist_id, QString * secretToken )
{

}

// Groups
void SoundCloudApi::getGroups( )
{
    QString url = urlRoot;
    url.append( urlGroups );


    startRequest( url );
}

void SoundCloudApi::getGroup( int group_id )
{
    QString url = urlRoot;
    url.append( urlGroups );
    url.append( "/" );
    url.append( group_id );


    startRequest( url );
}

void SoundCloudApi::getModeratorsForGroup( int group_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( group_id );
    url.append( urlModerators );


    startRequest( url );
}

void SoundCloudApi::getMembersForGroup( int group_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( group_id );
    url.append( urlMembers );


    startRequest( url );
}

void SoundCloudApi::getContributorsForGroup( int group_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( group_id );
    url.append( urlContributors );


    startRequest( url );
}

void SoundCloudApi::getUsersForGroup( int group_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( group_id );
    url.append( urlUsers );


    startRequest( url );
}

void SoundCloudApi::getTracksForGroup( int group_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( group_id );
    url.append( urlTracks );


    startRequest( url );
}

void SoundCloudApi::getPendingTracksForGroup( int group_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( group_id );
    url.append( urlPendingTracks );


    startRequest( url );
}

void SoundCloudApi::getTrackForGroup( int group_id, int track_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( group_id );
    url.append( urlTracks );
    url.append( "/" );
    url.append( track_id );


    startRequest( url );
}

void SoundCloudApi::addTrackForGroup( int group_id, Track * track )
{

}

void SoundCloudApi::deleteTrackForGroup( int group_id, Track * track )
{
}

void SoundCloudApi::getContributionsForGroup( int group_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( group_id );
    url.append( urlContributions );


    startRequest( url );
}

void SoundCloudApi::getContributionForGroup( int group_id, int track_id )
{
    QString url = urlRoot;
    url.append( urlPlaylists );
    url.append( "/" );
    url.append( group_id );
    url.append( urlContributions );
    url.append( "/" );
    url.append( track_id );


    startRequest( url );
}

void SoundCloudApi::addContributionForGroup( int group_id, Track * track )
{
}

void SoundCloudApi::deleteContributionForGroup( int group_id, Track * track )
{
}

// Comments
void SoundCloudApi::getComments( )
{
    QString url = urlRoot;
    url.append( urlComments );


    startRequest( url );
}

// Me


void SoundCloudApi::getMe( )
{
    QString url = urlRoot;
    url.append( urlMe );

    mReplyType = USER;

    startRequest( url );
}


void SoundCloudApi::getActivity( )
{
    QString url = urlRoot;
    url.append( urlMe );
    url.append( urlActivities );

    startRequest( url );
}


// Request
void SoundCloudApi::startRequest( QString& _strUrl )
{
    QNetworkRequest networkRequest = setSSLConfig();
    //QNetworkRequest networkRequest;

    _strUrl.append( ".json?oauth_token=" );
    _strUrl.append( mOAuthToken );


    qDebug() << "api request: " << _strUrl;

    networkRequest.setUrl( QUrl( _strUrl ) );

    mNetworkAccessManager->get( networkRequest );

    connect ( mNetworkAccessManager, SIGNAL( finished( QNetworkReply* ) ),
             this,           SLOT( slotHandleNetworkData( QNetworkReply* ) ),
            Qt::UniqueConnection );
}

QNetworkRequest SoundCloudApi::setSSLConfig()
{
    QNetworkRequest networkRequest;

    QSslConfiguration sslConfig = networkRequest.sslConfiguration();
    sslConfig.setPeerVerifyMode( QSslSocket::VerifyNone );
    sslConfig.setProtocol( QSsl::TlsV1 );

    QNetworkRequest request( networkRequest );
    request.setSslConfiguration( sslConfig );

    return request;
}

// Parsing method
void SoundCloudApi::slotHandleNetworkData( QNetworkReply* _Reply )
{

    if( !_Reply->error() )
    {
        qDebug() << "Reply received";

//        switch( mReplyType )
//        {
//        case USER:
//            parseUser( _Reply->readAll() );
//            break;

//        case TRACK:
            parseTrack( _Reply->readAll() );
  //          break;
//        }

        /*
        const QUrl url = _Reply->request().url();

        qDebug() << url.path();

        int i = url.path().lastIndexOf( "/" );
        QString type = url.path().mid( i + 1, url.path().length() - 6 );
        qDebug() << type;


        // ||
        if( !type.compare( "me" ) || !type.compare( "user" ) )
        {
            parseUser( _Reply->readAll() );
        }
        if( !type.compare( "track" ) )
        {
            parseTrack( _Reply->readAll() );
        }
        */
    }
    else
    {
        qDebug() << "Error: " << _Reply->errorString();
    }
}

void SoundCloudApi::parseUser( QByteArray response )
{
    qDebug() << "Parsing reply ...";

    QVariant json = mParser->parse( response );
    const QMap<QString, QVariant> map = json.toMap();
    User * user = new User( );

    if( map.contains( "id" ) )
    {
        user->mId = map.value( "id" ).toInt();
        qDebug() << "id: " << user->mId;
    }
    if( map.contains( "uri" ) )
    {
        user->mUri = map.value( "uri" ).toString();
        qDebug() << "uri: " << user->mUri;
    }
    if( map.contains( "permalink" ) )
    {
        user->mPermalink = map.value( "permalink" ).toString();
        qDebug() << "permalink: " << user->mPermalink;
    }
    if( map.contains( "permalink_url" ) )
    {
        user->mPermalinkUrl = map.value( "permalink_url" ).toString();
        qDebug() << "permalink_url: " << user->mPermalinkUrl;
    }
    if( map.contains( "avatar_url" ) )
    {
        user->mAvatarUrl = map.value( "avatar_url" ).toString();
        qDebug() << "avatar_url: " << user->mAvatarUrl;
    }
    if( map.contains( "username" ) )
    {
        user->mUsername = map.value( "username" ).toString();
        qDebug() << "username: " << user->mUsername;
    }
    if( map.contains( "country" ) )
    {
        user->mCountry = map.value( "country" ).toString();
        qDebug() << "country: " << user->mCountry;
    }
    if( map.contains( "full_name" ) )
    {
        user->mFullname = map.value( "full_name" ).toString();
        qDebug() << "fullname: " << user->mFullname;
    }
    if( map.contains( "description" ) )
    {
        user->mDescription = map.value( "description" ).toString();
        qDebug() << "description: " << user->mDescription;
    }
    if( map.contains( "online" ) )
    {
        user->mOnline = map.value( "online" ).toBool();
        qDebug() << "online: " << user->mOnline;
    }
    if( map.contains( "track_count" ) )
    {
        user->mTrackCount = map.value( "track_count" ).toInt();
        qDebug() << "track_count: " << user->mTrackCount;
    }
    if( map.contains( "playlist_count" ) )
    {
        user->mPlaylistCount = map.value( "playlist_count" ).toInt();
        qDebug() << "playlist_count: " << user->mPlaylistCount;
    }
    if( map.contains( "followers_count" ) )
    {
        user->mFollowersCount = map.value( "followers_count" ).toInt();
        qDebug() << "followers_count: " << user->mFollowersCount;
    }
    if( map.contains( "followings_count" ) )
    {
        user->mFollowingsCount = map.value( "followings_count" ).toInt();
        qDebug() << "followings_count: " << user->mFollowingsCount;
    }
    if( map.contains( "public_favorites_count" ) )
    {
        user->mPublicFavoritesCount = map.value( "public_favorites_count" ).toInt();
        qDebug() << "public_favorites_count: " << user->mPublicFavoritesCount;
    }
    if( map.contains( "city" ) )
    {
        user->mCity = map.value( "city" ).toString();
        qDebug() << "city: " << user->mCity;
    }
    if( map.contains( "discogs_name" ) )
    {
        user->mDiscogsName = map.value( "discogs_name" ).toString();
        qDebug() << "discogs_name: " << user->mDiscogsName;
    }
    if( map.contains( "myspace_name" ) )
    {
        user->mMyspaceName = map.value( "myspace_name" ).toString();
        qDebug() << "myspace_url: " << user->mMyspaceName;
    }
    if( map.contains( "website" ) )
    {
        user->mWebsite = map.value( "website" ).toString();
        qDebug() << "website: " << user->mWebsite;
    }
    if( map.contains( "website_title" ) )
    {
        user->mWebsiteTitle = map.value( "website_title" ).toString();
        qDebug() << "website_title: " << user->mWebsiteTitle;
    }
    if( map.contains( "private_playlists_count" ) )
    {
        user->mPrivatePlaylistCount = map.value( "private_playlist_count" ).toInt();
        qDebug() << "private_playist_count: " << user->mPrivatePlaylistCount;
    }
    if( map.contains( "private_tracks_count" ) )
    {
        user->mPrivateTracksCount = map.value( "private_tracks_count" ).toInt();
        qDebug() << "private_tracks_count: " << user->mPrivateTracksCount;
    }
    if( map.contains( "private_email_confirmed" ) )
    {
        user->mPrimaryEmailConfirmed = map.value( "private_email_confirmed" ).toBool();
        qDebug() << "primary_email_confirmed: " << user->mPrimaryEmailConfirmed;
    }
    if( map.contains( "plan" ) )
    {
        user->mPlan = map.value( "plan" ).toString();
        qDebug() << "plan: " << user->mPlan;
    }

    emit ( sigUserRequestFinished( user ) );
}

void SoundCloudApi::parseTrack( QByteArray response )
{
    qDebug() << "Parsing reply ...";

    QVariant json = mParser->parse( response );
    const QMap<QString, QVariant> map = json.toMap();
    Track *track = new Track( );

    if( map.contains( "id" ) )
    {
        track->mId = map.value( "id" ).toInt();
        qDebug() << "id: " << track->mId;
    }
    if( map.contains( "uri" ) )
    {
        track->mUri = map.value( "uri" ).toString();
        qDebug() << "uri: " << track->mUri;
    }
    if( map.contains( "permalink" ) )
    {
        track->mPermalink = map.value( "permalink" ).toString();
        qDebug() << "permalink: " << track->mPermalink;
    }
    if( map.contains( "permalink_url" ) )
    {
        track->mPermalinkUrl = map.value( "permalink_url" ).toString();
        qDebug() << "permalink_url: " << track->mPermalinkUrl;
    }
    if( map.contains( "title" ) )
    {
        track->mTitle = map.value( "title" ).toString();
        qDebug() << "title: " << track->mTitle;
    }
    if( map.contains( "created_at" ) )
    {
        track->mCreatedAt = map.value( "created_at" ).toString();
        qDebug() << "created_at: " << track->mCreatedAt;
    }
    if( map.contains( "sharing" ) )
    {
        track->mSharing = map.value( "sharing" ).toString();
        qDebug() << "shairing: " << track->mSharing;
    }
    if( map.contains( "purchase_url" ) )
    {
        track->mPurchaseUrl = map.value( "purchase_url" ).toString();
        qDebug() << "purchase_url: " << track->mPurchaseUrl;
    }
    if( map.contains( "artwork_url" ) )
    {
        track->mArtworkUrl = map.value( "artwork_url" ).toString();
        qDebug() << "artwork_url: " << track->mArtworkUrl;
    }
    if( map.contains( "description" ) )
    {
        track->mDescription = map.value( "description" ).toString();
        qDebug() << "description: " << track->mDescription;
    }
    if( map.contains( "label" ) )
    {
        //track->mLabel = map.value( "label" ).toMap();
        //qDebug() << "label: " << track->mLabel;
    }
    if( map.contains( "duration" ) )
    {
        track->mDuration = map.value( "duration" ).toInt();
        qDebug() << "duration: " << track->mDuration;
    }
    if( map.contains( "genre" ) )
    {
        track->mGenre = map.value( "genre" ).toString();
        qDebug() << "genre: " << track->mGenre;
    }
    if( map.contains( "shared_to_count" ) )
    {
        track->mSharedToCount = map.value( "shared_to_count" ).toInt();
        qDebug() << "shared_to_count: " << track->mSharedToCount;
    }
    if( map.contains( "tag_list" ) )
    {
        track->mTagList = map.value( "tag_list" ).toString();
        qDebug() << "tag_list: " << track->mTagList;
    }
    if( map.contains( "label_id" ) )
    {
        track->mLabelId = map.value( "label_id" ).toInt();
        qDebug() << "label_id: " << track->mLabelId;
    }
    if( map.contains( "label_name" ) )
    {
        track->mLabelName = map.value( "label_name" ).toString();
        qDebug() << "label_name: " << track->mLabelName;
    }
    if( map.contains( "license" ) )
    {
        track->mLicense = map.value( "licence" ).toString();
        qDebug() << "license: " << track->mLicense;
    }
    if( map.contains( "release" ) )
    {
        track->mRelease = map.value( "release" ).toInt();
        qDebug() << "release: " << track->mRelease;
    }
    if( map.contains( "release_day" ) )
    {
        track->mReleaseDay = map.value( "release_day" ).toInt();
        qDebug() << "release_day: " << track->mReleaseDay;
    }
    if( map.contains( "release_month" ) )
    {
        track->mReleaseMonth = map.value( "release_month" ).toInt();
        qDebug() << "release_month: " << track->mReleaseMonth;
    }
    if( map.contains( "release_year" ) )
    {
        track->mReleaseYear = map.value( "release_year" ).toInt();
        qDebug() << "release_year: " << track->mReleaseYear;
    }
    if( map.contains( "state" ) )
    {
        track->mState = map.value( "state" ).toString();
        qDebug() << "state: " << track->mState;
    }
    if( map.contains( "streamable" ) )
    {
        track->mStreamable = map.value( "streamable" ).toBool();
        qDebug() << "streamable: " << track->mStreamable;
    }
    if( map.contains( "track_type" ) )
    {
        track->mTrackType = map.value( "track_type" ).toString();
        qDebug() << "track_type: " << track->mTrackType;
    }
    if( map.contains( "waveform_url" ) )
    {
        track->mWaveformUrl = map.value( "waveform_url" ).toString();
        qDebug() << "release_year: " << track->mWaveformUrl;
    }
    if( map.contains( "download_url" ) )
    {
        track->mDownloadUrl = map.value( "download_url" ).toString();
        qDebug() << "download_url: " << track->mDownloadUrl;
    }
    if( map.contains( "stream_url" ) )
    {
        track->mStreamUrl = map.value( "stream_url" ).toString();
        track->mStreamUrl.append("?client_id="CLIENT_ID);
        qDebug() << "stream_url: " << track->mStreamUrl;
    }
    if( map.contains( "bpm" ) )
    {
        track->mBpm = map.value( "bpm" ).toInt();
        qDebug() << "bpm: " << track->mBpm;
    }
    if( map.contains( "commentable" ) )
    {
        track->mCommentable = map.value( "commentable" ).toBool();
        qDebug() << "commentable: " << track->mCommentable;
    }
    if( map.contains( "downloadable" ) )
    {
        track->mDownloadable = map.value( "downloadable" ).toBool();
        qDebug() << "downloadable: " << track->mDownloadable;
    }
    if( map.contains( "isrc" ) )
    {
        track->mIsrc = map.value( "isrc" ).toString();
        qDebug() << "isrc: " << track->mIsrc;
    }
    if( map.contains( "key_signature" ) )
    {
        track->mKeySignature = map.value( "key_signature" ).toString();
        qDebug() << "key_signature: " << track->mKeySignature;
    }
    if( map.contains( "comment_count" ) )
    {
        track->mCommentCount = map.value( "comment_count" ).toInt();
        qDebug() << "comment_count: " << track->mCommentCount;
    }
    if( map.contains( "download_count" ) )
    {
        track->mDownloadCount = map.value( "download_count" ).toInt();
        qDebug() << "download_count: " << track->mDownloadCount;
    }
    if( map.contains( "playback_count" ) )
    {
        track->mPlaybackCount = map.value( "playback_count" ).toInt();
        qDebug() << "playback_count: " << track->mPlaybackCount;
    }
    if( map.contains( "favoritings_count" ) )
    {
        track->mFavoritingsCount = map.value( "favoritings_count" ).toInt();
        qDebug() << "favoritings_count: " << track->mFavoritingsCount;
    }
    if( map.contains( "original_format" ) )
    {
        track->mOriginalFormat = map.value( "original_format" ).toString();
        qDebug() << "original_format: " << track->mOriginalFormat;
    }
    if( map.contains( "created_with" ) )
    {
        //track->mCreatedWith = map.value( "created_with" ).toMap();
        //qDebug() << "created_with: " << track->mCreatedWith;
    }
    if( map.contains( "asset_data" ) )
    {
        track->mAssetData = map.value( "asset_data" ).toInt();
        qDebug() << "asset_data: " << track->mAssetData;
    }
    if( map.contains( "artwork_data" ) )
    {
        track->mArtworkData = map.value( "artwork_data" ).toString();
        qDebug() << "artwork_data: " << track->mArtworkData;
    }

    emit ( sigTrackRequestFinished( track ) );
}
