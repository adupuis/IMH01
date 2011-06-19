#ifndef TRACK_H
#define TRACK_H

#include "label.h"
#include "app.h"

class Track
{
public:
    Track();

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
    QString mState;
    bool mStreamable;
    QString mTrackType;
    QString mWaveformUrl;
    QString mDownloadUrl;
    QString mStreamUrl;
    int mBpm;
    bool mCommentable;
    bool mDownloadable;
    QString mIsrc;
    QString mKeySignature;
    int mCommentCount;
    int mDownloadCount;
    int mPlaybackCount;
    int mFavoritingsCount;
    QString mOriginalFormat;
    App mCreatedWith;
    QString mAssetData;
    QString mArtworkData;

};

#endif // TRACK_H
