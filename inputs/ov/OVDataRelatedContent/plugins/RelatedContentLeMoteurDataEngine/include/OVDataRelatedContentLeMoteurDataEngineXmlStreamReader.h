#ifndef OVDATARELATEDCONTENTLEMOTEURDATAENGINEXMLHANDLER_H
#define OVDATARELATEDCONTENTLEMOTEURDATAENGINEXMLHANDLER_H

#include <QtXml/QXmlStreamReader>
#include <QtCore/QList>
#include "OVDataRelatedContentResult.h"

class OVDataRelatedContentLeMoteurDataEngineXmlStreamReader : public QXmlStreamReader
{
public:
    OVDataRelatedContentLeMoteurDataEngineXmlStreamReader();
    bool parseContent();
    QString providerLogo();
    QList<OVDataRelatedContentResult *> & getResults();
private:
    // Functions
    void readEntry();
    void readMediaGroup();
    // Variables
    QList<OVDataRelatedContentResult *> m_resultList;
    QStringRef m_providerLogoUrl;
};

#endif // OVDATARELATEDCONTENTLEMOTEURDATAENGINEXMLHANDLER_H
