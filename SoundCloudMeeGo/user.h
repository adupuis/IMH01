#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    int mId;
    QString mUri;
    QString mPermalink;
    QString mPermalinkUrl;
    QString mAvatarUrl;
    QString mUsername;
    QString mCountry;
    QString mFullname;
    QString mDescription;
    bool mOnline;
    int mTrackCount;
    int mPlaylistCount;
    int mFollowersCount;
    int mFollowingsCount;
    int mPublicFavoritesCount;
    QString mCity;
    QString mDiscogsName;
    QString mMyspaceName;
    QString mWebsiteTitle;
    QString mWebsite;

    // me specific fields
    int mPrivatePlaylistCount;
    int mPrivateTracksCount;
    bool mPrimaryEmailConfirmed;
    QString mPlan;

};

#endif // USER_H
