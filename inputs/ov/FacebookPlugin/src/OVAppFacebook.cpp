#include <OVAppFacebook.h>

#include <OVDebugCommon.h>


namespace OVSchematics
{
// public

        AppFacebook::AppFacebook()
        :
        AppPluginVirtualInterface()
        {
            OV_AUTOLOG();

            this->registerInterface( m_appFacebookInterface );
            this->registerJob( m_appFacebookJob );
            m_appFacebookJob.setInterface( &m_appFacebookInterface );
            this->loadDatabase();
            this->loadSqlFile( "0.1", "./resources/plugins/FacebookPlugin.sql" );
            m_appFacebookJob.connectToDatabase();
            m_appFacebookJob.startJob();
            m_appFacebookInterface.initialInterface();

            OV_AUTOLOG();
        }

        AppFacebook::~AppFacebook()
        {
            OV_AUTOLOG();


            OV_AUTOLOG();
        }


// protected

        QString AppFacebook::getName()
        {
            return "facebook";
        }

        void AppFacebook::initialize()
        {
        }
}

Q_EXPORT_PLUGIN2(FacebookPlugin, OVSchematics::AppFacebook);



