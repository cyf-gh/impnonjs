#include <base\Str.h>
#include "ImpNonJSImpl.h"
#include "..\include\Enum.h"
using namespace stImpJSEnum;
using namespace stInput;

using namespace __stLibEvent;

namespace stJSEvents {
	ST_EVENT( EventNoParam ) pBTN[MAX_JS_BUTTON] = { 0 };
	
	ST_EVENT( EventNoParam ) pPOV[MAX_JS_POV] = { 0 };

	ST_EVENT( EventStick )	 pLEFT = NULL;
	ST_EVENT( EventStick )	 pRIGHT = NULL;
}

using namespace stJSEvents;

#define POV_DEFAULT	-1
#define POV_UP		0
#define POV_DOWN	18000
#define POV_LEFT	27000
#define POV_RIGHT	9000

#define PROC_JS_BUTTONS( index ) \
	if ( ( m_pJS->State().rgbButtons[index] & 0x80 ) && ( false == switchBtn ) ) { \
		ST_CALL_EVENT( pBTN[index], () ); \
		switchBtn = true; \
	} \
	if ( m_pJS->State().rgbButtons[index] & 0x80 ) { \
		switchBtn = false; \
	}

#define JS_POV_DOWN( index ) \
	if( switchPov == false ) { \
		ST_CALL_EVENT( pPOV[0], () ); \
		switchPov = true; \
		break; \
	}

#define PROC_JS_POV() \
	switch ( m_pJS->State().rgdwPOV[0] ) { \
	case POV_DEFAULT : \
		switchPov = false; \
		break; \
	case POV_UP : \
		JS_POV_DOWN( 0 ); \
	case POV_DOWN : \
		JS_POV_DOWN( 1 ); \
	case POV_LEFT : \
		JS_POV_DOWN( 2 ); \
	case POV_RIGHT : \
		JS_POV_DOWN( 3 ); \
	}

#define PROC_JS_STICKS() \
	ST_CALL_EVENT( pLEFT, ( m_pJS->State().lX, m_pJS->State().lY, m_pJS->State().lZ ) ); \
	ST_CALL_EVENT( pRIGHT, ( m_pJS->State().lRx, m_pJS->State().lRy, m_pJS->State().lRz ) );

ImpNonJSImpl::ImpNonJSImpl()
	: m_pdev( new stInputDev( GetModuleHandle( NULL ) ) ), 
	  m_pJS( new stInputJS( GetConsoleWindow(), ( IDirectInput8 * )m_pdev->Handle(), ST_DEVICE_OWN ) ),
	  m_window( NULL ) { }

ImpNonJSImpl::~ImpNonJSImpl() {
	delete m_pdev;
	delete m_pJS;
}

un32 ImpNonJSImpl::Loop() {
	bool switchBtn = false;
	bool switchPov = false;

	while ( true ) {
		for ( nbus i = 0; i < MAX_JS_BUTTON; i++ ) {
			PROC_JS_BUTTONS( i );
		}
		
			PROC_JS_POV();

			PROC_JS_STICKS();

		Sleep( 1 ); // reduce usage of CPU
	}
}

void ImpNonJSImpl::SetButtonEvents( const un32 index, ST_EVENT( EventNoParam ) pfunc ) {
	pBTN[index] = pfunc;
}

void ImpNonJSImpl::SetStickEvents( bool isLeft, ST_EVENT( EventStick ) pfunc ) {
	if ( isLeft ) {
		pLEFT = pfunc;
	} else {
		pRIGHT= pfunc;
	}
}

void ImpNonJSImpl::SetPovEvents( const un32 index, ST_EVENT( EventNoParam ) pfunc ) {
	pPOV[index] = pfunc;
}

HANDLE ImpNonJSImpl::Reset( const stStrW &className, const stStrW &windowName ) {
	m_window = FindWindow( className.Data(), windowName.Data() ); 

	if ( NULL == m_window ) {
		wprintf( L"[ERR] Assigned Window Not Found. ClassName : [%ls].  WindowName : [%ls]", className.Data(), windowName.Data() );
	} else {
		wprintf( L"[OK]  Assigned Window Found.     ClassName : [%ls].  WindowName : [%ls]", className.Data(), windowName.Data() );	
	}
	return m_window;
}