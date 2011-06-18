/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#ifndef OVDATARELATEDCONTENTRESULT_H
#define OVDATARELATEDCONTENTRESULT_H

#include <QtCore/QMap>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

#include <QFuture>
#include <QFutureWatcher>

/*!
* \class OVDataRelatedContentResult
* \brief This class represent one of the related content query's results. Usually you got a list of OVDataRelatedContentResult.
*/

class OVDataRelatedContentResult : public QObject
{
    Q_OBJECT
public:
    /*!
          The RCSField enum contains all affectable value for a result.
          \sa setValue().
        */
    enum RCSField {
        Id,
        Title,
        Description,
        ThumbnailsLinks,
        Thumbnails,
        Rating,
        MaxRating,
        Links,
        DefaultLink,
        Duration,
        Provider,
    };
    OVDataRelatedContentResult();
    virtual ~OVDataRelatedContentResult();
    /*!
          Returns the id of the current result.
          \warning The id can be assigned by the provider, in this case you cannot know the form or type it will be. It is wise to treat all IDs as QString.
        */

    QString id();
    /*!
          Returns the title of the current result.
        */
    QString title();
    /*!
          Returns the description of the current result.
        */
    QString description();
    /*!
          Returns the list of thumbnails url links of the current result.
        */
    QStringList thumbnailsLinks();


    QString thumbnailFilepath();

    /*!
          Returns the rating of the current result. This value is to be put in relation with the maximum rating possible for its provider.
          \sa maxRating().
        */
    float rating();
    /*!
          Returns the maximum rating possible on the provider. Default value is 5.
          \sa rating().
        */
    int maxRating();
    /*!
          Returns the duration of the content in seconds.
        */
    int duration();
    /*!
          Returns the default link to the default content for this result. If the provider do not support the "default content" feature, the first link to the content is returned here.
        */
    QString defaultContentLink();
    /*!
          Returns the list of links to the content.
        */
    QStringList links();
    /*!
          Return the stored data for the given RCSField.
          \warning All developpers should prefer to use the dedicated methods ! But for some purpose (storing thumbnails for example) it can be easier to directly access to the QVariant.
        */
    QVariant getValue(RCSField);
    //public slots:
    /*!
          Allows to manually set data to values.
        */
    void setValue( RCSField, const QVariant & );

    void                        cacheThumbnail( const QByteArray& _rData, const QString& _rstrFilename, const QString& _rstrBaseDir );

    static QString              convertHTTPLinkToFilename( const QString& _rstrPath );

signals:
    void contentUpdated( OVDataRelatedContentResult::RCSField );

private slots:

    void                        onThumbnailCachingFinished();

private:

    bool                        processThumbnailCaching( const QByteArray& _rData, const QString& _rstrFilename, const QString& _rstrBaseDir );

    QMap<RCSField,QVariant>     m_data;

    struct FuturePair
    {
        QFuture<bool>           m_future;
        QFutureWatcher<bool>    m_watcher;
    };

    FuturePair*                 m_pThumbnailCachingWatcher;
};

#endif // OVDATARELATEDCONTENTRESULT_H
