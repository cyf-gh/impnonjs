#include <core\Core.h>
#include <utils\Dir.h>
#include <utils\Converter.h>
#pragma comment( lib, "stLibx86.lib" )
using namespace stLibCore;
using namespace stLibUtils;

#include <impnonjs.h>
#pragma comment( lib, "INJSx86.lib" )

#include "app-plugins.h"
#include "app-plugins-dic.h"
#include "app-events.h"
#include "app-logs.h"

#define ST_NO_WINDOW_FOUND -1

#define ST_LOAD_EVT( ptr_type, ptr_member, func_symbol ) \
	pack.ptr_member = ( ST_EVENT( ptr_type ) )GetProcAddress( m_dllHs[i], func_symbol ); \
	if ( NULL == pack.ptr_member ) { \
		oneFails = true; \
		printf( ST_INJS_LOGERR_FUNCLOADFAILED, func_symbol ); \
	} else { \
		printf( ST_INJS_LOGOK_FUNCLOADED, func_symbol ); \
	}

void stAppPlugins::loadPlugins() {
	for ( un32 i = 0; i < m_dllHs.size(); ++i ) {
		stEventPack pack;
		bool oneFails = false;
		wchar_t *className = NULL;
		wchar_t *titleName = NULL;

		// bad code, isn't it?
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
			m_pjs->AppTargetWindow( className, titleName );
			m_packs.push_back( pack );
		}
	}
	printf( "------DLL Function Loading Finished\nSUCCEED:[ %d ]\nFAILED[ %d ]\n\n", m_packs.size(), ( m_dllHs.size() - m_packs.size() ) );
}

un32 stAppPlugins::Loop() {
	n32 windowIndex = ST_NO_WINDOW_FOUND;

	while ( true ) {
		m_pjs->Proc( windowIndex );
		if ( windowIndex != ST_NO_WINDOW_FOUND ) {
			// set all events
			for ( un32 i = 0; i < 8; i++) {
				m_pjs->SetEvents_Button( i, m_packs[windowIndex].pBTN[i] ); 
			}
			for ( un32 i = 0; i < 4; i++) {
				m_pjs->SetEvents_Pov( i, m_packs[windowIndex].pPOV[i] ); 
			}
			for ( un32 i = 0; i < ST_MAX_STATE; ++i ) {
				m_pjs->SetEvents_State( (STATE)i, m_packs[windowIndex].pSTATE[i] ); 
			}
			m_pjs->SetEvents_Stick( true, m_packs[windowIndex].pLEFT );
			m_pjs->SetEvents_Stick( false, m_packs[windowIndex].pRIGHT );
			
			un32 behavior = 0;
			ST_CALL_EVENT( m_packs[windowIndex].p_Behavior, ( behavior ) );

			m_pjs->SetBehavior( ( stInputEnum::BEHAVIOR )behavior );

			windowIndex = ST_NO_WINDOW_FOUND;
		}
	}
}

stAppPlugins::stAppPlugins() : m_pjs( NULL ) {
	std::vector<stStrW> fileNames;
	stLibUtils::stDir dir;

	m_pjs = CreateImpNonJS();
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

stEventPack::stEventPack() : p_PassInstance( NULL ), pLEFT( NULL ), pRIGHT( NULL ), p_ClassName( NULL ), p_TitleName( NULL ), p_Behavior( NULL ) {
	for ( un32 i = 0; i < 8; ++i ) {
		pBTN[i] = NULL;
	}
	for ( un32 i = 0; i < 4; ++i ) {
		pPOV[i] = NULL;
	}
	for ( un32 i = 0; i < ST_MAX_STATE; ++i ) {
		pSTATE[i] = NULL;
	}
}
