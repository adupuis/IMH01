/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#include "OVDataRelatedContentYouTubeDataEngine.h"
#include "OVDataRelatedContentYouTubeDataEngineXmlStreamReader.h"
#include "OVDataRelatedContentResult.h"
#include <QDebug>
#include <QImage>

#include <OVDebugCommon.h>
#include <OVDebugAllocation.h>


OVDataRelatedContentYouTubeDataEngine::OVDataRelatedContentYouTubeDataEngine()
{
    m_queriesMap.clear();
    m_expectedImageMap.clear();
    m_pManager = OV_NEW(Domaine_DATA) QNetworkAccessManager(this);
    OV_CONNECT(m_pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReplyFinished(QNetworkReply*)), Qt::QueuedConnection);
}

OVDataRelatedContentYouTubeDataEngine::~OVDataRelatedContentYouTubeDataEngine()
{
    //OVDataRelatedContentResult freed outside

    for(QMap<QNetworkReply*, quint16>::const_iterator it = m_queriesMap.begin(); it != m_queriesMap.end(); ++it)
    {
        delete it.key(); //not OV_NEW'd since allocated by QT
    }

    SAFE_OV_DELETE(m_pManager);
}


QString OVDataRelatedContentYouTubeDataEngine::providerName()
{
    return QString("YouTubeDataEngine");
}

void OVDataRelatedContentYouTubeDataEngine::getRelatedContent( const QStringList & p_query, quint16 _iChecksum, unsigned int _iMaxResult )
{
    //        qDebug() << "CALL OVDataRelatedContentYouTubeDataEngine::getRelatedVideos";
    QString query_string = p_query.join("+");
    QNetworkReply* reply = m_pManager->get( QNetworkRequest(QUrl("http://gdata.youtube.com/feeds/api/videos?q="+query_string+"&max-results="+QString::number(_iMaxResult)+"&orderby=relevance")) );

    //        qDebug() << "  Query: " << query_string << " Checksum: " << checksum;
    m_queriesMap.insert(reply,_iChecksum);
}

void OVDataRelatedContentYouTubeDataEngine::onReplyFinished( QNetworkReply* _pReply )
{
    if( _pReply->header(QNetworkRequest::ContentTypeHeader).toString().contains("xml") )
    {
        quint16 iQueryID = m_queriesMap.take( _pReply );

        OV_MSG( "...::onReplyFinished for query %d\n", iQueryID );

        QList<OVDataRelatedContentResult *> lResults;

        processReplyXML( *_pReply, lResults );

        delete _pReply; //not OV_NEW'd since allocated by QT

        // NDAL : emit this signal AFTER we worked with the list, since the memory might be deleted by the user
        emit( relatedContentResultsReady( iQueryID, lResults ) );
    }
    else
    if( _pReply->header(QNetworkRequest::ContentTypeHeader).toString().startsWith("image/") )
    {
        // NDAL : use m_expectedImageMap.take( reply ) instead of m_expectedImageMap.value( reply ) in order to remove the entry from the map, since it is no longer needed
        // hence use an ASSERT to check the entry is alright
        OV_ASSERT( m_expectedImageMap.contains( _pReply ) );

        OVDataRelatedContentResult* pResult = m_expectedImageMap.take( _pReply );

        QString strFilename = OVDataRelatedContentResult::convertHTTPLinkToFilename( pResult->thumbnailsLinks().at( 0 ) );

        QByteArray data = _pReply->readAll();

        pResult->cacheThumbnail( data, strFilename, "/tmp/mnt/diska3/ov/img/youtube/" );
    }
}

void OVDataRelatedContentYouTubeDataEngine::processReplyXML( QNetworkReply& _rReply, QList<OVDataRelatedContentResult *>& out_rlResults )
{
    // since NetworkAccessManager is NOT thread safe, we should NOT thread this function

    OVDataRelatedContentYouTubeDataEngineXmlStreamReader parser;

    parser.addData( _rReply.readAll() );
    parser.parseContent();

    out_rlResults = parser.getResults();

    foreach( OVDataRelatedContentResult* pResult, out_rlResults )
    {
        // foreach(QString imgUrl, r->thumbnailsLinks())
        // NDAL - just DL the FIRST image !!!
        if( pResult->thumbnailsLinks().size() > 0 )
        {
            QString strFilename = OVDataRelatedContentResult::convertHTTPLinkToFilename( pResult->thumbnailsLinks().at( 0 ) );
            QString strFilePath = strFilename;
            strFilePath.prepend( "/tmp/mnt/diska3/ov/img/youtube/" );

            // avoid re downloading it if it is already available in our temp dir
            if( ! QFile::exists( strFilePath ) )
            {
                QString imgUrl = pResult->thumbnailsLinks().at(0);
                //                                qDebug() << "    Downloading img: " << imgUrl;
                QNetworkReply* tmpReply = m_pManager->get( QNetworkRequest(QUrl( imgUrl ) ) );
                m_expectedImageMap.insert(tmpReply,pResult);
            }
            else
            {
                pResult->setValue( OVDataRelatedContentResult::Thumbnails, strFilename );
            }
        }
    }
}


Q_EXPORT_PLUGIN2(OVDataRelatedContentYouTubeDataEngine, OVDataRelatedContentYouTubeDataEngine);
