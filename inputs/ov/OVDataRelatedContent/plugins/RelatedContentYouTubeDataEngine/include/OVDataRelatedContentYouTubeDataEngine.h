/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#ifndef OVDATARELATEDCONTENTYOUTUBEDATAENGINE_H
#define OVDATARELATEDCONTENTYOUTUBEDATAENGINE_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtNetwork>
#include "OVDataRelatedContentDataEngineInterface.h"

class OVDataRelatedContentYouTubeDataEngine : public OVDataRelatedContentDataEngineInterface
{
    Q_OBJECT
    Q_INTERFACES( OVDataRelatedContentDataEngineInterface )

public:
    OVDataRelatedContentYouTubeDataEngine();
    ~OVDataRelatedContentYouTubeDataEngine();

    void                        getRelatedContent( const QStringList & _rQuery, quint16 _iChecksum, unsigned int _iMaxResult );
    QString                     providerName();

private slots:
    void                        onReplyFinished(QNetworkReply*);

private:

    void                        processReplyXML( QNetworkReply& _rReply, QList<OVDataRelatedContentResult *>& out_rlResults );


    QNetworkAccessManager *                                 m_pManager;

    QMap< QNetworkReply *, quint16 >                        m_queriesMap;
    QMap< quint16, QList<OVDataRelatedContentResult* > >    m_resultsMap;
    QMap< QNetworkReply *, OVDataRelatedContentResult* >    m_expectedImageMap;
};

#endif // OVDATARELATEDCONTENTYOUTUBEDATAENGINE_H
