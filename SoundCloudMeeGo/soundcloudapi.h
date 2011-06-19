#ifndef SOUNDCLOUNDAPI_H
#define SOUNDCLOUNDAPI_H

#include <QObject>

#include <QtCore>
#include <QtNetwork>
#include <QNetworkAccessManager>

#include "comment.h"
#include "group.h"
#include "playlist.h"
#include "track.h"
#include "user.h"
#include "include/qjson/parser.h"

#define CLIENT_ID   "ef7c3301f5a463034354f0bfa1ee0236"

class SoundCloudApi : public QObject
{
    Q_OBJECT

public:
    SoundCloudApi( );
    ~SoundCloudApi();

    void setOAuthToken( QString & _sToken );


    // User
    void getUsers( );
    void getUser( int user_id );
    void getTrackForUser( int user_id );
    void getPlaylistForUser( int user_id );
    void getFollowingsForUser( int user_id );
    void getFollowingForUser( int user_id, int following_id );
    void addFollowingForUser( int user_id, User * following );
    void deleteFollowingForUser( int user_id, User * following );
    void getFollowersForUser( int user_id );
    void getFollowerForUser( int user_id, int follower_id );
    void getCommentsForUser( int user_id );
    void getFavoritesForUser( int user_id );
    void getFavoriteForUser( int user_id, int favorite_id );
    void addFavoriteForUser( int user_id, Track * favorite );
    void deleteFavoriteForUser( int user_id, Track * favorite );
    void getGroupsForUser( int user_id );

    // Track
    void getTracks( );
    void getTrack( int track_id );
    void addTrack( Track * track );
    void deleteTrack( Track * track );
    void getCommentsForTrack( int track_id );
    void addCommentForTrack( int track_id, Comment * comment );
    void deleteCommentForTrack( int track_id, Comment * comment );
    void getCommentForTrack( int track_id, int comment_id );
    void getFavoritersForTrack( int track_id );
    void getFavoriterForTrack( int track_id, int favoriter_id );
    // TODO: Vérifier le type de retour
    void getPermissionsForTrack( int track_id );
    // TODO: Vérifier le type de retour
    void getPermissionForTrack( int track_id, int permission_id );
    void addPermissionForTrack( int user_id, User * permission );
    void deletePermissionForTrack( int user_id, User * permission );
    void getSecretTokenForTrack( int track_id );
    void setSecretTokenForTrack( int track_id, QString * secretToken );

    // Playlists
    void getPlaylists( );
    // TODO: Vérifier le type de retour
    void getPermissionsForPlaylist( int playlist_id );
    // TODO: Vérifier le type de retour
    void getPermissionForPlaylist( int playlist_id, int permission_id );
    void addPermissionForPlaylist( int playlist_id, User * permission );
    void deletePermissionForPlaylist( int playlist_id, User * permission );
    void getSecretTokenForPlaylist( int playlist_id );
    void setSecretTokenForPlaylist( int playlist_id, QString * secretToken );

    // Groups
    void getGroups( );
    void getGroup( int group_id );
    void getModeratorsForGroup( int group_id );
    void getMembersForGroup( int group_id );
    void getContributorsForGroup( int group_id );
    void getUsersForGroup( int group_id );
    void getTracksForGroup( int group_id );
    void getPendingTracksForGroup( int group_id );
    void getTrackForGroup( int group_id, int track_id );
    void addTrackForGroup( int group_id, Track * track );
    void deleteTrackForGroup( int group_id, Track * track );
    void getContributionsForGroup( int group_id );
    void getContributionForGroup( int group_id, int track_id );
    void addContributionForGroup( int group_id, Track * track );
    void deleteContributionForGroup( int group_id, Track * track );

    // Comments
    void getComments( );

    // Me
    void getMe( );
    void getActivity( );

private slots:

    void slotHandleNetworkData( QNetworkReply * _Reply );

private:

    void startRequest( QString& _strUrl );
    User * parseUser( QByteArray response );
    Track * parseTrack( QByteArray response );

    QNetworkRequest setSSLConfig( );

    QNetworkAccessManager* mNetworkAccessManager;
    //QNetworkReply* mNetworkReply;
    QJson::Parser * mParser;
    QString mOAuthToken;

    const QString urlRoot;
    const QString urlUsers;
    const QString urlTracks;
    const QString urlPlaylists;
    const QString urlFollowings;
    const QString urlFollowers;
    const QString urlGroups;
    const QString urlComments;
    const QString urlFavorites;
    const QString urlFavoriters;
    const QString urlPermissions;
    const QString urlSecretToken;
    const QString urlModerators;
    const QString urlMembers;
    const QString urlContributors;
    const QString urlPendingTracks;
    const QString urlContributions;
    const QString urlMe;
    const QString urlConnections;
    const QString urlActivities;
    const QString urlApps;
    const QString urlResolve;

signals:

    void sigUserRequestFinished( User * user );
    void sigTrackRequestFinished( Track * track );

};

#endif // SOUNDCLOUNDAPI_H
