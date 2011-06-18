/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#ifndef OVDATARELATEDCONTENTYOUTUBEDATAENGINE_H
#define OVDATARELATEDCONTENTLEMOTEURDATAENGINE_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtNetwork>
#include "OVDataRelatedContentDataEngineInterface.h"

class OVDataRelatedContentLeMoteurDataEngine : public OVDataRelatedContentDataEngineInterface
{
    Q_OBJECT
    Q_INTERFACES(OVDataRelatedContentDataEngineInterface)

public:
    OVDataRelatedContentLeMoteurDataEngine();
    ~OVDataRelatedContentLeMoteurDataEngine();

    void                        getRelatedContent( const QStringList & _rQuery, quint16 _iChecksum, unsigned int _iMaxResult );

    QString                     providerName();
private slots:
    void                        replyFinished(QNetworkReply*);
private:
    QNetworkAccessManager *                             m_manager;
    QMap<QNetworkReply *, quint16>                      m_replyMap;
    QMap<QNetworkReply *, OVDataRelatedContentResult *> m_resultMap;
};

#endif // OVDATARELATEDCONTENTLEMOTEURDATAENGINE_H
