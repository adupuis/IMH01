#ifndef GROUP_H
#define GROUP_H

#include "label.h"

class Group
{
public:
    Group();

    int mId;
    QString mUri;
    QString mCreatedAt;
    QString mPermalink;
    QString mPermalinkUrl;
    QString mArtworkUrl;
    QString mName;
    QString mDescription;
    QString mShortDescription;
    Label mCreator;
};

#endif // GROUP_H
