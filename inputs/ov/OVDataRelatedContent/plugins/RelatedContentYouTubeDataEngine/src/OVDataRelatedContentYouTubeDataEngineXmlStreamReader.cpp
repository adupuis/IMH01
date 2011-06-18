/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#include "OVDataRelatedContentYouTubeDataEngineXmlStreamReader.h"
#include <QtCore/QDebug>

#include <OVDebugCommon.h>
#include <OVDebugAllocation.h>

OVDataRelatedContentYouTubeDataEngineXmlStreamReader::OVDataRelatedContentYouTubeDataEngineXmlStreamReader()
{
    m_lResults.clear();
}

QList<OVDataRelatedContentResult *> & OVDataRelatedContentYouTubeDataEngineXmlStreamReader::getResults()
{
    return m_lResults;
}

bool OVDataRelatedContentYouTubeDataEngineXmlStreamReader::parseContent()
{
    //        qDebug() << "start reading XML";
    while( !atEnd() )
    {
        readNext();
        //                qDebug() << "readNext() called and element is " << name();
        if( isStartElement() )
        {
            if( name() == "entry" )
            {
                //                                qDebug() << "OVDataRelatedContentYouTubeDataEngineXmlStreamReader::parseContent() => new entry";
                OVDataRelatedContentResult* pResult = OV_NEW(Domaine_DATA) OVDataRelatedContentResult;
                pResult->setValue( OVDataRelatedContentResult::Provider, "YouTubeDataEngine" );
                m_lResults.append( pResult );
                readEntry();
            }
            else
            if( name() == "logo" )
            {
                readNext();
                m_providerLogoUrl = text();
                //                                qDebug() << "OVDataRelatedContentYouTubeDataEngineXmlStreamReader::parseContent() logo URL is " << m_providerLogoUrl;
            }
            //                        else
            //                                raiseError(QObject::tr("The file is not a YouTube content result file."));
        }
    }
    if( hasError() )
    {
        qWarning() << "XML has errors, at line " << lineNumber() << " col. " << columnNumber() << " error message is: " << errorString() ;
    }

    return !error();
}

void OVDataRelatedContentYouTubeDataEngineXmlStreamReader::readEntry()
{
    OVDataRelatedContentResult* pResult = m_lResults.last();

    while( ! atEnd() )
    {
        readNext();
        if( ! name().isEmpty() && isStartElement() )
        {
            if( name() == "id" )
            {
                readNext();
                pResult->setValue(OVDataRelatedContentResult::Id,text().toString());
            }
            else if( name() == "title" )
            {
                if( attributes().value("type").toString().contains("text") )
                {
                    readNext();
                    pResult->setValue( OVDataRelatedContentResult::Title, text().toString() );
                    //                                        qDebug() << "=> CONTENT(title): " << pResult->title();
                }
            }
            else if( name() == "content" )
            {
                if( attributes().value("type").toString().contains("text") )
                {
                    readNext();
                    pResult->setValue( OVDataRelatedContentResult::Description, text().toString() );
                }
            }
            else if( name() == "rating" )
            {
                pResult->setValue(OVDataRelatedContentResult::Rating, attributes().value("average").toString() );
                pResult->setValue(OVDataRelatedContentResult::MaxRating, attributes().value("max").toString());
                //                                qDebug() << "=> CONTENT: " << pResult->title() << " score: " << pResult->rating() << "/" << pResult->maxRating();
            }
            else if( name() == "group" && prefix() == "media" )
            {
                readMediaGroup();
                //                                qDebug() << "AFTER MEDIA: " << pResult->defaultContentLink() << " ; " << pResult->duration();
            }
        }
        else
        if( name() == "entry" && isEndElement() )
        {
            break;
        }
    }
}

void OVDataRelatedContentYouTubeDataEngineXmlStreamReader::readMediaGroup()
{
    Q_ASSERT(isStartElement() && name() == "group" && prefix() == "media" );

    OVDataRelatedContentResult* pResult = m_lResults.last();

    while( ! atEnd() )
    {
        readNext();

        if( ! name().isEmpty() && isStartElement() )
        {
            if( prefix() == "media" )
            {
                if( name() == "description" )
                {
                    readNext();
                    pResult->setValue(OVDataRelatedContentResult::Description,text().toString());
                }
                else
                if( name() == "content" && attributes().value("medium").toString() == "video" )
                {
                    QString url = attributes().value("url").toString();
                    pResult->setValue(OVDataRelatedContentResult::Links, pResult->links() << url );

                    if( attributes().value("isDefault").toString() == "true" )
                    {
                        pResult->setValue( OVDataRelatedContentResult::DefaultLink, url );
                    }
                }
                else
                if( name() == "thumbnail" )
                {
                    pResult->setValue(OVDataRelatedContentResult::ThumbnailsLinks, pResult->thumbnailsLinks() << attributes().value("url").toString());
                }
            }
            else
            if( prefix() == "yt" )
            {
                if( name() == "duration" )
                {
                    pResult->setValue(OVDataRelatedContentResult::Duration, attributes().value("seconds").toString() );
                }
            }
        }
        else
        if( name() == "group" && isEndElement() )
        {
            break;
        }
    }
}
