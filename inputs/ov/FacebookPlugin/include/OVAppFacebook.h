#ifndef OVSCHEMATICSAPPFACEBOOK_H
#define OVSCHEMATICSAPPFACEBOOK_H


#include <OVSchematicsAppPluginVirtualInterface.h>

#include <OVAppFacebookInterface.h>
#include <OVAppFacebookJob.h>


namespace OVSchematics
{
        class AppFacebook : public OVSchematics::AppPluginVirtualInterface
        {
            Q_OBJECT
            Q_INTERFACES( OVSchematics::AppPluginVirtualInterface )

        public:
                AppFacebook();

                ~AppFacebook();


        protected:
                virtual QString getName();

                virtual void initialize();


        private:
                OVAppFacebookInterface  m_appFacebookInterface;

                AppFacebookJob          m_appFacebookJob;



         //       virtual void onVisualizationChanged(AppPluginInterface::AppVisualisationMode parVisualisation);

         //       virtual void refresh(const Notification* parNotif = NULL);
        };
}

#endif // OVSCHEMATICSAPPFACEBOOK_H
