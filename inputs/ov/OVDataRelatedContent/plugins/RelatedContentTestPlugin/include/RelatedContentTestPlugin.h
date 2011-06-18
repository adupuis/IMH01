/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#ifndef RELATEDCONTENTTESTPLUGIN_H
#define RELATEDCONTENTTESTPLUGIN_H

#include <QObject>
#include "OVDataRelatedContentDataEngineInterface.h"

class RelatedContentTestPlugin : public QObject, OVDataRelatedContentDataEngineInterface
{
    Q_OBJECT
    Q_INTERFACES(OVDataRelatedContentDataEngineInterface)

public:
    RelatedContentTestPlugin();
    void getRelatedVideos( const QStringList & p_query );
    QString providerName();
};

#endif // RELATEDCONTENTTESTPLUGIN_H
