#include "OVDataRelatedContentLeMoteurDataEngineXmlStreamReader.h"
#include <QtCore/QDebug>
#include <OVDebugCommon.h>
#include <OVDebugAllocation.h>

OVDataRelatedContentLeMoteurDataEngineXmlStreamReader::OVDataRelatedContentLeMoteurDataEngineXmlStreamReader()
{
    m_resultList.clear();
}

QList<OVDataRelatedContentResult *> & OVDataRelatedContentLeMoteurDataEngineXmlStreamReader::getResults()
{
    return m_resultList;
}

bool OVDataRelatedContentLeMoteurDataEngineXmlStreamReader::parseContent()
{
    while (!atEnd())
    {
        readNext();
        if (isStartElement()) {
            if (name() == "R"){
                OVDataRelatedContentResult *result = OV_NEW(Domaine_DATA) OVDataRelatedContentResult;
                m_resultList.append( result );
                readEntry();
            }
            else if( name() == "logo" ){
                readNext();
                m_providerLogoUrl = text();
                //                                qDebug() << "OVDataRelatedContentYouTubeDataEngineXmlStreamReader::parseContent() logo URL is " << m_providerLogoUrl;
            }
            //                        else
            //                                raiseError(QObject::tr("The file is not a YouTube content result file."));
        }
    }
    if(hasError())
        qWarning() << "XML has errors, at line " << lineNumber() << " col. " << columnNumber() << " error message is: " << errorString() ;
    return !error();
}

void OVDataRelatedContentLeMoteurDataEngineXmlStreamReader::readEntry()
{
    OVDataRelatedContentResult *result = m_resultList.last();
    while( ! atEnd() )
    {
        readNext();
/*
<NBMATCH>2</NBMATCH>
<URLPAGE>http://videos.tf1.fr/jt-13h/le-succes-des-ventes-directes-a-la-ferme-5854860.html</URLPAGE>
<URLVIDEO>http://videos.tf1.fr/jt-13h/le-succes-des-ventes-directes-a-la-ferme-5854860.html</URLVIDEO>
<BROADCASTER>tf1</BROADCASTER>
<DATEVIDEO>1274090400</DATEVIDEO>
<STARTVIDEO></STARTVIDEO>
<URLPLAYER>http://dh.search.ke.voila.fr:80/r?url=http%3A%2F%2Fvideos.tf1.fr%2Fjt-13h%2Fle-succes-des-ventes-directes-a-la-ferme-5854860.html&amp;rurl=http%3A%2F%2Fvideos.tf1.fr%2Fjt-13h%2Fle-succes-des-ventes-directes-a-la-ferme-5854860.html&amp;id=2515000001274357614444750&amp;ap=1&amp;profil=lemoteur&amp;bhv=&amp;rdata=broadcaster%3Atf1%20ferme&amp;numa=0&amp;nump=0&amp;lg=%3F%3F&amp;v=01&amp;salt=828898&amp;part=videos_mediasearch&amp;key=4b1e0be5ebd787b922c1ef5077fb1946&amp;md5sum=e35a2fe3c878703b30b4761dece9d1e3</URLPLAYER>
<TYPE>ext</TYPE>

*/

/*
                Thumbnails,
                MaxRating,
                Links,
                DefaultLink : no default link here
                Provider,
*/
        if( ! name().isEmpty() && isStartElement() )
        {
            if( name() == "ID" )
            {
                readNext();
                result->setValue(OVDataRelatedContentResult::Id,text().toString());
                //qDebug() << "ID : " << result->id();
            }
            else if( name() == "RK" )
            {
                readNext();
                result->setValue(OVDataRelatedContentResult::Rating, text().toString());
                //qDebug() << "RK : " << result->rating();
                //TODO : set a fixed max rating ?
                //result->setValue(OVDataRelatedContentResult::MaxRating, attributes().value("max").toString());
                //qDebug() << "=> CONTENT: " << result->title() << " score: " << result->rating() << "/" << result->maxRating();
            }
            else if( name() == "TITLEPAGE" )
            {
                readNext();
                result->setValue( OVDataRelatedContentResult::Title, text().toString() );
                //qDebug() << "=> TITLEPAGE : " << result->title();
            }
            else if( name() == "DESCVIDEO" )
            {
                readNext();
                result->setValue( OVDataRelatedContentResult::Description, text().toString() );
                //qDebug() << "DESCVIDEO : " << result->description();
            }
            else if( name() == "IMAGEVIDEO" )
            {
                readNext();
                result->setValue(OVDataRelatedContentResult::ThumbnailsLinks, result->thumbnailsLinks() << text().toString());
                //qDebug() << "IMAGEVIDEO : " << result->thumbnailsLinks();
            }
            else if( name() == "LENGTHVIDEO" ){
                readNext();
                result->setValue(OVDataRelatedContentResult::Duration, text().toString());
                //qDebug() << "LENGTHVIDEO : " << result->duration();
            }
            else if( name() == "URLVIDEO" ){
                readNext();
                result->setValue(OVDataRelatedContentResult::Links, result->links() << text().toString() );
                //qDebug() << "URLVIDEO : " << result->links();
            }
            else if( name() == "BROADCASTER" ){
                readNext();
                result->setValue( OVDataRelatedContentResult::Provider, text().toString());
                //qDebug() << "BROADCASTER : " << result->getValue(OVDataRelatedContentResult::Provider);
            }

        }
        else if( name() == "R" && isEndElement() )
            break;
    }
}

void OVDataRelatedContentLeMoteurDataEngineXmlStreamReader::readMediaGroup(){
        Q_ASSERT(isStartElement() && name() == "group" && prefix() == "media" );
        OVDataRelatedContentResult *result = m_resultList.last();
        while( ! atEnd() ){
                readNext();
                if( ! name().isEmpty() && isStartElement() ){
                        if( name() == "description" && prefix() == "media" ){
                                readNext();
                                result->setValue(OVDataRelatedContentResult::Description,text().toString());
                        }
                        else if( name() == "content" && prefix() == "media" && attributes().value("medium").toString() == "video" ){
                                QString url = attributes().value("url").toString();
                                result->setValue(OVDataRelatedContentResult::Links, result->links() << url );
                                if( attributes().value("isDefault").toString() == "true" )
                                        result->setValue( OVDataRelatedContentResult::DefaultLink, url );
                        }
                        else if( name() == "description" && prefix() == "media" ){
                                readNext();
                                result->setValue(OVDataRelatedContentResult::Description,text().toString());
                        }
                        else if( name() == "thumbnail" && prefix() == "media" ){
                                result->setValue(OVDataRelatedContentResult::ThumbnailsLinks, result->thumbnailsLinks() << attributes().value("url").toString());
                        }
                        else if( name() == "duration" && prefix() == "yt" ){
                                result->setValue(OVDataRelatedContentResult::Duration, attributes().value("seconds").toString() );
                        }
                }
                else if( name() == "group" && isEndElement() ){
                        break;
                }
        }
}
