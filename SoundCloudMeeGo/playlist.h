#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "label.h"

class Playlist
{
public:
    Playlist();

    int mId;
    QString mTitle;
    QString mPermalink;
    QString mPermalinkUrl;
    QString mUri;
    QString mCreatedAt;
    QString mSharing;
    QString mPurchaseUrl;
    QString mArtworkUrl;
    QString mDescription;
    Label mLabel;
    long mDuration;
    QString mGenre;
    int mSharedToCount;
    QString mTagList;
    int mLabelId;
    QString mLabelName;
    QString mLicense;
    int mRelease;
    int mReleaseDay;
    int mReleaseMonth;
    int mReleaseYear;
    QString mEan;
    QString mPlaylistType;

};

#endif // PLAYLIST_H
