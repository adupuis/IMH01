/*
* Author: Arnaud Dupuis (a.dupuis@infinityperl.org)
* Copyright: Orange Vallee
*/

#include "OVDataRelatedContent.h"
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>
#include <QtGui/QImage>
#include "OVDebugCommon.h"
#include <OVDebugAllocation.h>

OVDataRelatedContent::OVDataRelatedContent(const QStringList & p_dirList) :
    QObject()
{
        // Hardcoding the seed makes the random number generator predictible but we really don't give a sh*t about that.
        qsrand(1234567890);
        m_pluginDirectoriesList.clear();
        m_pluginList.clear();
        m_loaderList.clear();
        if( p_dirList.isEmpty() )
                m_pluginDirectoriesList << "./";
        if( p_dirList.size() > 0 ){
//                qDebug() << "Plugin directories: " << p_dirList;
                m_pluginDirectoriesList << p_dirList;
        }
}
OVDataRelatedContent::~OVDataRelatedContent()
{
    //unload the loaded plugins
    foreach(QPluginLoader* loader, m_loaderList)
    {
        if(!loader->unload())
        {
            OV_ERROR("Unable to unload plugin : %s", loader->errorString().toUtf8().constData());
        }
        else
        {
            OV_PRINT("Plugin successfully unloaded.");
        }
        OV_DELETE(loader);
    }
}

//QStringList & OVDataRelatedContent::urlEncode(QStringList & p_list){
//        p_list.replaceInStrings("%","%25");
//        p_list.replaceInStrings("$","%24");
//        p_list.replaceInStrings("&","%26");
//        p_list.replaceInStrings("+","%2B");
//        p_list.replaceInStrings(",","%2C");
//        p_list.replaceInStrings("/","%2F");
//        p_list.replaceInStrings(":","%3A");
//        p_list.replaceInStrings(";","%3B");
//        p_list.replaceInStrings("=","%3D");
//        p_list.replaceInStrings("?","%3F");
//        p_list.replaceInStrings("@","%40");
//        p_list.replaceInStrings(" ","%20");
//        p_list.replaceInStrings("'","%22");
//        p_list.replaceInStrings("\"","%22");
//        p_list.replaceInStrings("<","%3C");
//        p_list.replaceInStrings(">","%3E");
//        p_list.replaceInStrings("#","%23");
//        p_list.replaceInStrings("{","%7B");
//        p_list.replaceInStrings("}","%7D");
//        p_list.replaceInStrings("|","%7C");
//        p_list.replaceInStrings("\\","%5C");
//        p_list.replaceInStrings("^","%5E");
//        p_list.replaceInStrings("~","%7E");
//        p_list.replaceInStrings("[","%5B");
//        p_list.replaceInStrings("]","%5D");
//        p_list.replaceInStrings("`","%60");
//        return p_list;
//}

void OVDataRelatedContent::addPluginDirectory(const QString & p_dirName){
        m_pluginDirectoriesList << p_dirName;
}

void OVDataRelatedContent::addPluginDirectories( const QStringList & p_dirList ){
        m_pluginDirectoriesList << p_dirList;
}

QStringList OVDataRelatedContent::loadPlugins()
{
    QStringList loadedPlugins;
    foreach( QString tmpDir, m_pluginDirectoriesList )
    {
//                qDebug() << "About to look for plugin in directory: " << tmpDir ;
        QDir tmpPluginsDir = QDir( tmpDir );
        tmpPluginsDir.cd( tmpDir );
        foreach (QString fileName, tmpPluginsDir.entryList(QDir::Files))
        {
            if( fileName.endsWith(".so") )
            {
                //qDebug() << "Loading plugin: " << fileName;
                QPluginLoader* loader = OV_NEW(Domaine_DATA) QPluginLoader(tmpPluginsDir.absoluteFilePath(fileName));
                loader->setLoadHints(QLibrary::ResolveAllSymbolsHint);
                QObject *plugin = loader->instance();
                if (plugin)
                {
                    loadedPlugins << fileName;
                    m_pluginList << qobject_cast<OVDataRelatedContentDataEngineInterface *>( plugin );
                    connect(plugin,
                            SIGNAL(relatedContentResultsReady(quint16, QList<OVDataRelatedContentResult *> & )),
                            this,
                            SIGNAL(relatedContentResultsReady(quint16, QList<OVDataRelatedContentResult *> & ))
                            );
                    OV_MSG("Plugin : %s loaded", fileName.toUtf8().constData());
                    m_loaderList.append(loader);
                }
                else
                {
                    OV_ERROR("Unable to load plugin : \"%s\"\nError : %s", fileName.toUtf8().constData(), loader->errorString().toUtf8().constData());
                    OV_DELETE(loader);
                }
            }
        }
    }
    return loadedPlugins;
}

quint16 OVDataRelatedContent::getRelatedContent( const QStringList & _rQuery, unsigned int _iMaxResult )
{
    // NDAL : execute this section of code in another thread

//        qDebug() << "CALL OVDataRelatedContent::getRelatedContent";
        QString query_string = _rQuery.join("+");
        quint16 iChecksum = qChecksum( query_string.toUtf8().constData(), query_string.size() ) + rand();

        // NDAL : why COPY ?
        QStringList encoded_query = _rQuery;
//        encoded_query = urlEncode(encoded_query);
        foreach( OVDataRelatedContentDataEngineInterface* pPlugin, m_pluginList )
        {
                pPlugin->getRelatedContent( encoded_query, iChecksum, _iMaxResult );
        }
        return iChecksum;
}
