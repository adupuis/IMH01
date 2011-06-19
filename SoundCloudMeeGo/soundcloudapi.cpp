#include "soundcloudapi.h"

SoundCloudApi::SoundCloudApi( )
    : urlRoot ( "https://api.soundcloud.com" )
    , urlUsers ( "/users" )
    , urlTracks ( "/track" )
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
    url.append( urlUsers );
    url.append( "/" );
    url.append( track_id );


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

    qDebug() << _strUrl;

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
        const QUrl url = _Reply->request().url();

        qDebug() << url.path();

        int i = url.path().lastIndexOf( "/" );
        QString type = url.path().mid( i + 1, url.path().length() - 6 );
        qDebug() << type;


        // ||
        if( !type.compare( "me" ) )
        {
            parseUser( _Reply->readAll() );
        }
    }
    else
    {
        qDebug() << "Error: " << _Reply->errorString();
    }
}

User * SoundCloudApi::parseUser( QByteArray response )
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

    emit ( sigUsersRequestFinished( user ) );
}
