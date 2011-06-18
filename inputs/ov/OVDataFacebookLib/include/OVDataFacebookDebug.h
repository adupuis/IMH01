#ifndef OVDATAFACEBOOKDEBUG_H
#define OVDATAFACEBOOKDEBUG_H

#include <OVDebugCommon.h>
#include <OVSmartParser.h>

#define OVFACEBOOKCONFIGINI "OVFacebookConfig.ini"

//#ifdef _DEBUG
//static int s_iOVFBDebugLevel = 2;
//#else
//static int s_iOVFBDebugLevel = 1;
//#endif

//parseFile Qt
//s_iOVFBDebugLevel

static int getOVFBDebugLevel()
{
    QString strConfigToParse = OVFACEBOOKCONFIGINI;
    OVSmartParser* parser = new OVSmartParser( strConfigToParse );

    int iDebugLevel = parser->getConfigValue( "fb_debug_level" ).toInt();

    return iDebugLevel;
}

static int s_iOVFBDebugLevel = getOVFBDebugLevel();

#define OVFB_FATAL_ERROR( format, ... ) do \
{ \
    OV_FATAL_ERROR( format, ##__VA_ARGS__ ); \
} while( 0 )

#define OVFB_ERROR( format, ... ) do \
{ \
    if( s_iOVFBDebugLevel >= 1 ) \
    { \
        OV_ERROR(format,##__VA_ARGS__); \
    } \
} while( 0 )

#define OVFB_WARNING( format, ... ) do \
{ \
    if( s_iOVFBDebugLevel >= 2 ) \
    { \
        OV_WARNING( format, ##__VA_ARGS__ ); \
    } \
} while( 0 )

#define OVFB_IMPORTANT_MSG( format, ... ) do \
{ \
    if( s_iOVFBDebugLevel >= 2 ) \
    { \
        OV_IMPORTANT_MSG( format, ##__VA_ARGS__ ); \
    } \
} while( 0 )

#define OVFB_FLASHING_MSG( format, ... ) do \
{ \
    if( s_iOVFBDebugLevel >= 3 ) \
    { \
        OV_FLASHING_MSG( format, ##__VA_ARGS__ ); \
    } \
} while( 0 )

#define OVFB_MSG( format, ... ) do \
{ \
    if( s_iOVFBDebugLevel >= 3 ) \
    { \
        OV_MSG( format, ##__VA_ARGS__ ); \
    } \
} while( 0 )

#define OVFB_TRACE( format, ... ) do \
{ \
    if( s_iOVFBDebugLevel >= 4 ) \
    { \
        OV_TRACE( format, ##__VA_ARGS__ ); \
    } \
} while( 0 )


#endif // OVDATAFACEBOOKDEBUG_H
