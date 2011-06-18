/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#include "OVDataRelatedContentResult.h"
#include <QtGui/QImage>

#include <OVDebugCommon.h>
#include <OVDebugAllocation.h>

OVDataRelatedContentResult::OVDataRelatedContentResult()
:   m_pThumbnailCachingWatcher   ( NULL )
{
        m_data.clear();
}

OVDataRelatedContentResult::~OVDataRelatedContentResult(){}

QString OVDataRelatedContentResult::id(){
        return m_data.value(Id, "").toString();
}

QString OVDataRelatedContentResult::title()
{
        return m_data.value(Title, "").toString();
}

QString OVDataRelatedContentResult::description()
{
        return m_data.value(Description, "").toString();
}

QStringList OVDataRelatedContentResult::thumbnailsLinks()
{
    return m_data.value(ThumbnailsLinks,QStringList()).toStringList();
}

QString OVDataRelatedContentResult::thumbnailFilepath()
{
    return m_data.value( Thumbnails, "" ).toString();
}

float OVDataRelatedContentResult::rating(){
        return m_data.value(Rating, -1).toDouble();
}

int OVDataRelatedContentResult::maxRating(){
        return m_data.value(MaxRating,5).toInt();
}

int OVDataRelatedContentResult::duration(){
        return m_data.value(Duration, 0).toInt();
}

QString OVDataRelatedContentResult::defaultContentLink(){
        return m_data.value(DefaultLink, "").toString();
}

QStringList OVDataRelatedContentResult::links(){
        return m_data.value(Links, QStringList()).toStringList();
}

void OVDataRelatedContentResult::setValue( RCSField field, const QVariant & data){
//        qDebug("OVDataRelatedContentResult::setValue processing data and emitting signal");
        m_data.insert(field,data);
        emit( contentUpdated(field) );
}

QVariant OVDataRelatedContentResult::getValue(RCSField field){
        return m_data.value(field, QVariant());
}

void OVDataRelatedContentResult::cacheThumbnail( const QByteArray& _rData, const QString& _rstrFilename, const QString& _rstrBaseDir )
{
    SAFE_OV_DELETE( m_pThumbnailCachingWatcher );

    m_pThumbnailCachingWatcher = OV_NEW (Domaine_DATA) FuturePair();

    OV_CONNECT( & m_pThumbnailCachingWatcher->m_watcher, SIGNAL(finished()), this, SLOT( onThumbnailCachingFinished() ), Qt::QueuedConnection );

    m_pThumbnailCachingWatcher->m_future = QtConcurrent::run( this, & OVDataRelatedContentResult::processThumbnailCaching, _rData, _rstrFilename, _rstrBaseDir );
    m_pThumbnailCachingWatcher->m_watcher.setFuture( m_pThumbnailCachingWatcher->m_future );

}

void OVDataRelatedContentResult::onThumbnailCachingFinished()
{
    bool bSuccess = m_pThumbnailCachingWatcher->m_watcher.future().result();

    if( bSuccess )
    {
        QString strFilename = convertHTTPLinkToFilename( thumbnailsLinks().at( 0 ) );
        setValue( OVDataRelatedContentResult::Thumbnails, strFilename );
    }

    SAFE_OV_DELETE( m_pThumbnailCachingWatcher );
}


QString OVDataRelatedContentResult::convertHTTPLinkToFilename( const QString& _rstrPath )
{
    // convert the thumbnail image http link to a custom & equivalent filepath !
    QString strPath = _rstrPath;

    strPath.replace( '/', '_' );
    strPath.replace( ':', '_' );

    return strPath;
}

// =============================================== 2ND THREAD ===============================================

bool OVDataRelatedContentResult::processThumbnailCaching( const QByteArray& _rData, const QString& _rstrFilename, const QString& _rstrBaseDir )// OVDataRelatedContentResult* _pResult )
{
    // execute in another thread !
    QImage image = QImage::fromData( _rData );

    // scale down the image if it is "too big"
    const int iMaxThumbnailWidth = 512;

    if( image.width() > iMaxThumbnailWidth )
    {
        OV_MSG( "resizing image from (%d,%d) to (%d,?)...\n", image.width(), image.height(), iMaxThumbnailWidth );

        image = image.scaledToWidth( iMaxThumbnailWidth );
    }
    // convert the thumbnail image http link to a custom & equivalent filepath !
    QString strFilePath = _rstrFilename;
    strFilePath.prepend( _rstrBaseDir );

    QString strDirPath = QFileInfo( strFilePath ).path();
    QDir dir( strDirPath );

    if( ! dir.exists() )
    {
        dir.mkpath( strDirPath );
    }

    bool bSuccess = image.save( strFilePath );

    OV_MSG( "Writing image to %s -- success %d\n", strFilePath.toUtf8().constData(), (int) bSuccess );

    return bSuccess;
}
