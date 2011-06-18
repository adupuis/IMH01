/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#ifndef OVDATARELATEDCONTENTYOUTUBEDATAENGINEXMLHANDLER_H
#define OVDATARELATEDCONTENTYOUTUBEDATAENGINEXMLHANDLER_H

#include <QtXml/QXmlStreamReader>
#include <QtCore/QList>
#include "OVDataRelatedContentResult.h"

class OVDataRelatedContentYouTubeDataEngineXmlStreamReader : public QXmlStreamReader
{
public:
    OVDataRelatedContentYouTubeDataEngineXmlStreamReader();
    bool parseContent();
    QString providerLogo();
    QList<OVDataRelatedContentResult *> & getResults();
private:
    // Functions
    void readEntry();
    void readMediaGroup();
    // Variables
    QList<OVDataRelatedContentResult*>          m_lResults;
    QStringRef                                  m_providerLogoUrl;
};

#endif // OVDATARELATEDCONTENTYOUTUBEDATAENGINEXMLHANDLER_H
