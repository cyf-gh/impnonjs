#include <core\Core.h>
#include <utils\Dir.h>
#include <utils\Converter.h>
using namespace stLibCore;
using namespace stLibUtils;

#include <impnonjs.h>

#include "app-plugins.h"
#include "app-plugins-dic.h"
#include "app-events.h"
#include "app-logs.h"

#define ST_LOAD_EVT( ptr_type, ptr_member, func_symbol ) \
	pack.ptr_member = ( ST_EVENT( ptr_type ) )GetProcAddress( m_dllHs[i], func_symbol ); \
	if ( NULL == pack.ptr_member ) { \
		oneFails = true; \
		printf( ST_INJS_LOGERR_FUNCLOADFAILED, func_symbol ); \
	} else { \
		printf( ST_INJS_LOGOK_FUNCLOADED, func_symbol ); \
	}

void stAppPlugins::loadPlugins() {
	un32 totalSuccess = 0;

	if ( m_dllHs.size() == 0 ) {
		printf( "\n------No plugin"  );	
	}

	for ( un32 i = 0; i < m_dllHs.size(); ++i ) {
		stEventPack pack;
		bool oneFails = false;
		wchar_t *className = NULL;
		wchar_t *titleName = NULL;
		
		/* bad code, isn't it? */
		printf( "\n------Loading DLL[ %d ] Functions...\n", i );
		ST_LOAD_EVT( EventPassInstance, p_PassInstance, ST_PLUGIN_FUNC_FEEDINSTANCE );
		ST_CALL_EVENT( pack.p_PassInstance, ( ( void * )m_pjs ) );

		ST_LOAD_EVT( EventBehavior, p_Behavior, ST_PLUGIN_FUNC_BEHAVIOR );
		ST_LOAD_EVT( EventGetWStr, p_ClassName, ST_PLUGIN_FUNC_CLASSNAME );
		ST_LOAD_EVT( EventGetWStr, p_TitleName,	ST_PLUGIN_FUNC_TITLENAME );

		/* Events */
		ST_LOAD_EVT( EventNoParam, pBTN[0], ST_PLUGIN_FUNC_BTN0 );
		ST_LOAD_EVT( EventNoParam, pBTN[1], ST_PLUGIN_FUNC_BTN1 );
		ST_LOAD_EVT( EventNoParam, pBTN[2], ST_PLUGIN_FUNC_BTN2 );
		ST_LOAD_EVT( EventNoParam, pBTN[3], ST_PLUGIN_FUNC_BTN3 );
		ST_LOAD_EVT( EventNoParam, pBTN[4], ST_PLUGIN_FUNC_BTN4 );
		ST_LOAD_EVT( EventNoParam, pBTN[5], ST_PLUGIN_FUNC_BTN5 );
		ST_LOAD_EVT( EventNoParam, pBTN[6], ST_PLUGIN_FUNC_BTN6 );
		ST_LOAD_EVT( EventNoParam, pBTN[7], ST_PLUGIN_FUNC_BTN7 );

		ST_LOAD_EVT( EventNoParam, pPOV[0], ST_PLUGIN_FUNC_POV0 );
		ST_LOAD_EVT( EventNoParam, pPOV[1], ST_PLUGIN_FUNC_POV1 );
		ST_LOAD_EVT( EventNoParam, pPOV[2], ST_PLUGIN_FUNC_POV2 );
		ST_LOAD_EVT( EventNoParam, pPOV[3], ST_PLUGIN_FUNC_POV3 );

		ST_LOAD_EVT( EventHandle, pSTATE[0], ST_PLUGIN_FUNC_WINDOWFOUND );
		ST_LOAD_EVT( EventHandle, pSTATE[1], ST_PLUGIN_FUNC_WINDOWLOST );

		ST_LOAD_EVT( EventStick, pRIGHT, ST_PLUGIN_FUNC_RIGHT );
		ST_LOAD_EVT( EventStick, pLEFT,  ST_PLUGIN_FUNC_LEFT );
		
		ST_CALL_EVENT( pack.p_ClassName, ( &className ) );
		ST_CALL_EVENT( pack.p_TitleName, ( &titleName ) );\

		if ( oneFails ) {
			continue;	
		}
		// push window info by DLL INDEX that is the find window order is same as PACK.
		if ( className != NULL && titleName != NULL ) {
			++totalSuccess;
			m_pjs->AppTargetWindow( className, titleName );
			m_pjs->AppEventPack( pack );
		}
	}
	printf( "------DLL Function Loading Finished\nSUCCEED:[ %d ]\nFAILED[ %d ]\n\n", totalSuccess, ( m_dllHs.size() - totalSuccess ) );
}

stAppPlugins::stAppPlugins( stImpNonJS *pinterface ) : m_pjs( pinterface ) {
	std::vector<stStrW> fileNames;
	stLibUtils::stDir dir;

	dir.Cd( L"plugins" );
	dir.Ls( fileNames );
	
	for ( un32 i = 0; i < fileNames.size(); ++i ) {
		HINSTANCE dll = LoadLibraryW( fileNames[i].Data() );
		if ( dll != NULL ) {
			m_dllHs.push_back( dll );
			wprintf( ST_INJS_LOGOK_DLLLOADED, fileNames[i].Data(), i );
		} else {
			wprintf( ST_INJS_LOGERR_DLLLOADFAILED, fileNames[i].Data() );
		}
	} 
	loadPlugins();
}

stAppPlugins::~stAppPlugins() {
	for ( un32 i = 0; i < m_dllHs.size(); ++i ) {
		FreeLibrary( m_dllHs[i] );
	}
}
