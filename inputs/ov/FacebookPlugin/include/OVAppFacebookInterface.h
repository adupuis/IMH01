#ifndef OVAPPFACEBOOKINTERFACE_H
#define OVAPPFACEBOOKINTERFACE_H

#include <QSqlDatabase>

#include <QtNetwork>


#include <OVSchematicsBasicAppInterface.h>

#include <OVAppFacebookDBHandler.h>
#include <OVSocialBrowser.h>

#include <OVSmartParser.h>

namespace OVSchematics
{

    class OVAppFacebookInterface : public OVSchematics::BasicAppInterface
    {
        Q_OBJECT


    public:

        OVAppFacebookInterface();

        ~OVAppFacebookInterface();

        void                    setAppDatabase( QSqlDatabase* _appDatabase );

        void                    setCurrentUserId( QString _strCurrentUserId );

        void                    setCurrentUserEmail( QString _strCurrentUserEmail );

        void                    setCurrentUserPwd( QString _strCurrentUserPwd );

        void                    setCurrentFanPageId( QString _strCurrentFanPageId );

        void                    setFriendsLikesPictures( QStringList _friendsLikesPictures );

        void                    setFriendsLikesNumber( int _iFriendsLikesNumber );

        void                    initialInterface();

        void                    setFanPagesToLikeOrUnlike( QStringList _fanPagesToLikeOrUnlike );


    protected:

        virtual void    categoryChanged( OVGUI::GUIObject* _oldObjectSelected, OVGUI::GUIObject* _objectSelected );

        virtual void    onFocusCategory();

        virtual void    onFocusContent();


        void            setFeedContent( OVAppFacebookDBHandler* _appFacebookDBHandler, AppPluginContent* _pCurContent );

        void            setFanpageContent( OVAppFacebookDBHandler* _appFacebookDBHandler, AppPluginContent* _pCurContent );

        void            setInboxContent( OVAppFacebookDBHandler* _appFacebookDBHandler, AppPluginContent* _pCurContent );

        void            setFriendlistContent( OVAppFacebookDBHandler* _appFacebookDBHandler, AppPluginContent* _pCurContent );

        void            setPicture( AppPluginContent* _pCurContent, QString _strPictureFilepath, double _fX, double _fY, double _fW, double _fH );

        void            setText( AppPluginContent* _pCurContent, QString _strText, QColor _color, QFont _font, double _fX, double _fY, double _fW, double _fH,  int _iCanvasWidth, int _iCanvasHeight );


    private:

        virtual void    slotMessageFromJobReceived( QVariant _message );

        virtual void    messageFromUI( int _iID, QVariant _message );


        QString         insertSpacesIntoNumber( QString & _strNumber );

        void            createSocialBrowser();

        void            destroySocialBrowser();


    private slots:

        void slotHandleOAuthActionFailed( const int _iId, const QString& _strErrorMessage );

        void slotHandleLikeActionFailed( const int _iId, const QString& _strErrorMessage );

        void slotHandleUnlikeActionFailed( const int _iId, const QString& _strErrorMessage );


        void slotHandleAccessTokenRetrieved( const QString& _strAccessToken );

        void slotHandleLikeActionDone();

        void slotHandleUnlikeActionDone();

        void slotHandleSocialBrowserDestroyed();


    private:

            QSqlDatabase*                           m_pAppDatabase;

            OVSchematics::OVAppFacebookDBHandler*   m_pAppFacebookDBHandler;

            OVSocialBrowser*                        m_pSocialBrowser;

            QList<AppPluginContent*>                m_listContent;

            QList<OVGUI::Data*>                     m_catList;

            OVGUI::GUIImage*                        m_pArrowCat;

            QColor                                  m_black;

            QColor                                  m_orange;

            QColor                                  m_snow;

            QColor                                  m_green;

            QColor                                  m_cFocusColor;

            QFont                                   m_timeFont;

            QFont                                   m_defaultFont;

            QString                                 m_strCurrentUserId;

            QString                                 m_strCurrentUserEmail;

            QString                                 m_strCurrentUserPwd;

            QString                                 m_strCurrentFanPageId;

            QStringList                             m_fanPagesToLikeOrUnlike;

            QStringList                             m_socialEmailAccessToken;

            QStringList                             m_friendsLikesPictures;

            int                                     m_iIndexData;

            int                                     m_iFriendsLikesNumber;

    };

}

#endif // OVAPPFACEBOOKINTERFACE_H
