/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/


#ifndef OVDATARELATEDCONTENTDATAENGINEINTERFACE_H
#define OVDATARELATEDCONTENTDATAENGINEINTERFACE_H

#include <QtPlugin>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <OVDataRelatedContentResult.h>
#include <QtCore/QObject>

/*!
  This interface define the minimal requirement that a data engine for the OVDataRelatedContent library must have.
*/

class OVDataRelatedContentDataEngineInterface : public QObject {
        Q_OBJECT
public:
        virtual ~OVDataRelatedContentDataEngineInterface(){};
        /*!
          Plugin will receive a query and must start his work from here.
          The query is already HTML escaped by the OVDataRelatedContent library.
          This function have to communicate through signals/slots mechanism.
        */
        virtual void getRelatedContent( const QStringList & _rQuery, quint16 _iChecksum, unsigned int _iMaxResult ) = 0;
        /*!
          Return the provider name.
          Provider name is a unique name that can identify the data engine.
        */
        virtual QString providerName() = 0;
signals:
        /*!
          This signal have to be emitted when a query finished (when the remote server returns the XML for example).
          Data engines should not wait for all the ressources to be (down)loaded to emit this signal. They should emits it as soon as they have a list of related content.
        */
        void relatedContentResultsReady(quint16, QList<OVDataRelatedContentResult *> & );
};

Q_DECLARE_INTERFACE(OVDataRelatedContentDataEngineInterface, "com.orange-vallee.RelatedContent.OVDataRelatedContentDataEngineInterface/0.1")

#endif // OVDATARELATEDCONTENTDATAENGINEINTERFACE_H
