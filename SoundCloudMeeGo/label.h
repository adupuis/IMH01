#ifndef LABEL_H
#define LABEL_H

#include <QString>

class Label
{
public:
    Label();

    int mId;
    QString mPermalink;
    QString mPermalinkUrl;
    QString mUsername;
    QString mUri;
    QString mAvatarUrl;
};

#endif // LABEL_H
