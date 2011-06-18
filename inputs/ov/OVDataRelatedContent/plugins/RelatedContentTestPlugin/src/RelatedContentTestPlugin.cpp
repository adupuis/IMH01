/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#include "RelatedContentTestPlugin.h"

RelatedContentTestPlugin::RelatedContentTestPlugin()
{
}

QString providerName(){
        return QString("TestDataEngine");
}

void RelatedContentTestPlugin::getRelatedVideos( const QStringList & p_query ){
        qDebug("RelatedContentTestPlugin::getRelatedVideos() function called");
}

Q_EXPORT_PLUGIN2(RelatedContentTestPlugin, RelatedContentTestPlugin);
