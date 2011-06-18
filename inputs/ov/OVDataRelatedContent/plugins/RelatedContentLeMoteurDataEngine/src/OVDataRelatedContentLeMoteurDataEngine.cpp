#include "OVDataRelatedContentLeMoteurDataEngine.h"
#include "OVDataRelatedContentLeMoteurDataEngineXmlStreamReader.h"
#include "OVDataRelatedContentResult.h"
#include <QDebug>
#include <QtGui/QImage>

#include <OVDebugCommon.h>
#include <OVDebugAllocation.h>

OVDataRelatedContentLeMoteurDataEngine::OVDataRelatedContentLeMoteurDataEngine()
{
    m_replyMap.clear();
    m_resultMap.clear();
    m_manager = OV_NEW(Domaine_DATA) QNetworkAccessManager(this);
    OV_CONNECT(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)), Qt::QueuedConnection);
}

OVDataRelatedContentLeMoteurDataEngine::~OVDataRelatedContentLeMoteurDataEngine()
{
    for(QMap<QNetworkReply*, OVDataRelatedContentResult*>::const_iterator it = m_resultMap.begin(); it != m_resultMap.end(); ++it)
    {
        OV_DELETE(it.value());
    }

    for(QMap<QNetworkReply*, quint16>::const_iterator it = m_replyMap.begin(); it != m_replyMap.end(); ++it)
    {
        delete it.key(); //not OV_NEW'd since allocated by QT
    }

    if(m_manager)
    {
        OV_DELETE(m_manager);
    }
}

QString OVDataRelatedContentLeMoteurDataEngine::providerName()
{
        return QString("LeMoteurDataEngine");
}

void OVDataRelatedContentLeMoteurDataEngine::getRelatedContent( const QStringList & _rQuery, quint16 _iChecksum, unsigned int _iMaxResult )
{
//        qDebug() << "CALL OVDataRelatedContentLeMoteurDataEngine::getRelatedVideos";
        QString query_string = _rQuery.join("+");

/* API, source : eric.alexandre@orange-ftgroup.com

url de base: http://search.ke.voila.fr/fr/mediasearch/xml/fullxml?rtype=kw

Paramètres get :

kw=….   => obligatoire, les mots clés recherchés, url encodés
an=N    => nombre de réponses par page
ap=N   => numéro de page
sort=… => critère de tri

quelques exemples de tri :

sort=ke_rk:desc,dateVideo:desc  => tri par pertinence puis date

sort=broadcaster:asc,dateVideo:desc  => tri par chaine puis date

sort=titlePage:asc  => tri par titre de video

sort=dateVideo:desc,titlePage:asc  => tri par date puis par titre de video


quelques exemples de requetes :

rdata=concert+live
rdata=concert+live+broadcaster:orange2424video


Exemple de requete complete
http://search.ke.voila.fr/fr/mediasearch/xml/fullxml?ap=1&an=10&rtype=kw&rdata=broadcaster:tf1+ferme&sort=dateVideo
*/

        //QNetworkReply* reply = m_manager->get( QNetworkRequest(QUrl("http://search.ke.voila.fr/fr/mediasearch/xml/fullxml?ap=1&an=10&rtype=kw&rdata=broadcaster:tf1+ferme&sort=dateVideo")));
        QNetworkReply* reply = m_manager->get( QNetworkRequest(QUrl("http://search.ke.voila.fr/fr/mediasearch/xml/fullxml?rtype=kw&rdata="+query_string+"&ap=1&an=25&sort=ke_rk:desc")) );
//        qDebug() << "  Query: " << query_string << " Checksum: " << checksum;
        m_replyMap.insert( reply,_iChecksum );
}

void OVDataRelatedContentLeMoteurDataEngine::replyFinished(QNetworkReply* reply){
//        qDebug() << "CALL OVDataRelatedContentYouTubeDataEngine::replyFinished();";
        if( reply->header(QNetworkRequest::ContentTypeHeader).toString().contains("xml") ){
                OVDataRelatedContentLeMoteurDataEngineXmlStreamReader parser;
                QString data(reply->readAll());
//                qDebug() << data;
                parser.addData(data);
                parser.parseContent();
                QList<OVDataRelatedContentResult *> results = parser.getResults();
                emit( relatedContentResultsReady(m_replyMap.value(reply),results) );
                foreach( OVDataRelatedContentResult *r, results ){
                        foreach(QString imgUrl, r->thumbnailsLinks()){
//                                qDebug() << "    Downloading img: " << imgUrl;
                                QNetworkReply* tmpReply = m_manager->get( QNetworkRequest(QUrl( imgUrl ) ) );
                                m_resultMap.insert(tmpReply,r);
                        }
                }
        }
        else if( reply->header(QNetworkRequest::ContentTypeHeader).toString().startsWith("image/") ){
                OVDataRelatedContentResult *r = m_resultMap.value(reply);
                QList<QVariant> variantList = r->getValue(OVDataRelatedContentResult::Thumbnails).toList();
                variantList << reply->readAll();
  //              qDebug() << "Setting new image list. Size: " << variantList.size();
                r->setValue( OVDataRelatedContentResult::Thumbnails, variantList );
        }
}

Q_EXPORT_PLUGIN2(OVDataRelatedContentLeMoteurDataEngine, OVDataRelatedContentLeMoteurDataEngine);
