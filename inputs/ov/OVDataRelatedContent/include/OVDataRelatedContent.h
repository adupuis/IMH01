/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#ifndef OVDATARELATEDCONTENT_H
#define OVDATARELATEDCONTENT_H

#include <QtCore>
#include <OVDataRelatedContentResult.h>
#include <OVDataRelatedContentDataEngineInterface.h>

/*!
* \class OVDataRelatedContent
* \brief Front library to get related content.
* \details This class is the front and core library for getting related content. By itself this class cannot access related content. It rely on plugins (hereafter referred to as "data engine") to do the actual job.
\code
// Create a new object with a list of path were to search for plugins
new OVDataRelatedContent *rc = new OVDataRelatedContent( QStringList() << "./plugins/"  << "./lib/plugins/" << "./dataEngine/" );
// Load all loadable plugins
rc->loadPlugins();
// Connect the signal to its slot
connect(rc, SIGNAL(relatedContentResultsReady(quint16, QList<OVDataRelatedContentResult *> & )), this, SLOT( relatedContentResultsReadySlot(quint16, QList<OVDataRelatedContentResult *> & ) ) );
// Get some related content
int checkSum = rc->getRelatedContent("some stupid tv show on TF1 or M6 that gives Coca Cola brain ready time");
\endcode
*/

class OVDataRelatedContent : public QObject {
        Q_OBJECT
public:
        /*!
          Constructor takes a list of directory were to search for plugins to load.
          \param p_dirList a list of directories where to search for plugins/data engines.
          \note If p_dirList is empty the current directory is added as a plugin directory
        */
        OVDataRelatedContent(const QStringList & p_dirList = QStringList());
        ~OVDataRelatedContent();
        /*!
          Get related content from the query string.
          \param p_query a list of (key)words to search related content for.
          \return a quint16 encoded checksum. You are certain that this checksum will be passed to relatedContentResultsReady() signal at each call, and will never be used with any other requests.
          \sa relatedContentResultsReady()
        */
        quint16 getRelatedContent( const QStringList & p_query, unsigned int _iMaxResult );
public slots:
        /*!
          Add a single directory to the list of directories where libOVDataRelatedContent will search for plugins.
          \param p_dirName the path (relative or absolute) to the plugin directory.
        */
        void addPluginDirectory(const QString & p_dirName);
        /*!
          Add a list of directories to the list of directories where libOVDataRelatedContent will search for plugins.
          \param p_dirList the path (relative or absolute) to the plugin directory.
        */
        void addPluginDirectories( const QStringList & p_dirList );
        /*!
          When called, this method loads all detected plugins. This method returns the list of successfully loaded plugins.
          \return the list of successfully loaded plugins.
        */
        QStringList loadPlugins();
signals:
        /*!
          This signal is emitted each time there is results ready.
          The checksum of the initial query and the list of results are passed together with the signal.
        */
        void relatedContentResultsReady(quint16, QList<OVDataRelatedContentResult *> & );
private:
        QList<QPluginLoader*> m_loaderList;
        QStringList m_pluginDirectoriesList;
        QList<OVDataRelatedContentDataEngineInterface *> m_pluginList;
};

#endif // OVDATARELATEDCONTENT_H
