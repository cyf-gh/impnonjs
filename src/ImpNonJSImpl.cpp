#include <base\Str.h>
#include "ImpNonJSImpl.h"
#include "..\include\Enum.h"
#include <XInput.h>
#pragma comment( lib, "xinput.lib" )
using namespace stInputEnum;
using namespace stINJSEnum;
using namespace stInput;

#include "Logs.h"

using namespace __stLibEvent;

#define POV_DEFAULT	-1
#define POV_UP		0
#define POV_DOWN	18000
#define POV_LEFT	27000
#define POV_RIGHT	9000

#define JS_POV_DOWN( index ) \
	if( switchPov == false ) { \
		ST_CALL_EVENT( m_curPack.pPOV[index], () ); \
		switchPov = true; \
		break; \
	}

ImpNonJSImpl::ImpNonJSImpl()
	: m_pdev( new stInputDev( GetModuleHandle( NULL ) ) ), 
	  m_pJS( new stInputJS( GetConsoleWindow(), 0, ( IDirectInput8 * )m_pdev->Handle(), ST_DEVICE_SHARE ) ),
	  m_curActiveWindow( NULL ), m_isOnlyBehaveTop( true ) { 
	resetVib();
}

ImpNonJSImpl::~ImpNonJSImpl() {
	delete m_pdev;
	delete m_pJS;
}

bool ImpNonJSImpl::checkWindowExist( n32 &findWindowIndex ) {
	for ( un32 i = 0; i < m_titles.size(); i++ ) {
		m_curActiveWindow = FindWindowW( m_className[i].Data(), m_titles[i].Data() );
		if ( NULL == m_curActiveWindow ) {
			if ( ST_G_NOERRED ) {
				wprintf( ST_INJS_LOGERR_WINDOWNOTFOUND,  m_className[i].Data(), m_titles[i].Data() );
				if ( ( i + 1 ) == m_titles.size() ) { // last info
					ST_G_SETERRED();
				}
			} 
		} else {
			ST_G_SETNOERR();
			wprintf( L"\n" );
			wprintf( ST_INJS_LOGOK_WINDOWFOUND,  m_className[i].Data(), m_titles[i].Data() );
			wprintf( ST_INJS_LOGOK_STARTPROCWIN, m_curActiveWindow );
			findWindowIndex = i;
			return true;
		}
	}
	return false;
}

void ImpNonJSImpl::resetVib() {
	m_vib.leftMotorSpeed	= 0;
	m_vib.rightMotorSpeed	= 0;
	m_vib.timeDuration		= 0;
	m_vib.peffectFunc		= NULL;
}

void ImpNonJSImpl::procVib() {
	static XINPUT_VIBRATION xv;

	m_stopwatch.Resume();
	if ( m_stopwatch.GetElapsed() < m_vib.timeDuration ) {
		ST_CALL_EVENT( m_vib.peffectFunc, ( m_vib.leftMotorSpeed, m_vib.rightMotorSpeed ) );
		if ( m_vib.leftMotorSpeed > 65535 || m_vib.rightMotorSpeed > 65535 ) {
			m_vib.leftMotorSpeed	= 65535;
			m_vib.rightMotorSpeed	= 65535;
		}
	} else {
		m_stopwatch.Reset();
		m_stopwatch.Pause();
		resetVib();
	}
	xv.wLeftMotorSpeed	= m_vib.leftMotorSpeed;
	xv.wRightMotorSpeed = m_vib.rightMotorSpeed;

	XInputSetState( 0, &xv );
}

void ImpNonJSImpl::Proc() {
	bool switchBtn[MAX_JS_BUTTON] = { false, false, false ,false, false, false, false, false };
	bool switchPov;
	n32 index = -1;
	
	while ( true ) {

	Sleep( 1 ); // reduce usage of CPU
	/* process window */
	if ( m_isOnlyBehaveTop ) {
		if ( ( m_curActiveWindow == NULL ) ) { // window doesn't exist
			if ( checkWindowExist( index ) ) {
				m_curPack = m_packs[index];
				ST_CALL_EVENT( m_curPack.pSTATE[ST_STATE_WINDOWFOUND], ( m_curActiveWindow ) );
			}
			continue;
		}
		if ( 0 == IsWindow( ( HWND )m_curActiveWindow ) ) { // handle is illegal
			ST_CALL_EVENT( m_curPack.pSTATE[ST_STATE_WINDOWLOST], ( m_curActiveWindow ) );
			m_curActiveWindow = NULL;
			wprintf( ST_INJS_LOGERR_LOSTWINDOW );
			continue;
		}
		if ( GetForegroundWindow() != m_curActiveWindow ) { // is non-foreground window
			continue;
		}
	}
	/* process buttons */
	for ( nbus i = 0; i < MAX_JS_BUTTON; i++ ) {
		if ( ( m_pJS->State().rgbButtons[i] & 0x80 ) && ( false == switchBtn[i] ) ) {
			ST_CALL_EVENT( m_curPack.pBTN[i], () );
			switchBtn[i] = true;
		} \
		if ( !( m_pJS->State().rgbButtons[i] & 0x80 ) ) {
			switchBtn[i] = false;
		}
	}
	/* process povs */	
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
	/* process sticks */	
	ST_CALL_EVENT( m_curPack.pLEFT, ( m_pJS->State().lX, m_pJS->State().lY, m_pJS->State().lZ ) ); \
	ST_CALL_EVENT( m_curPack.pRIGHT, ( m_pJS->State().lRx, m_pJS->State().lRy, m_pJS->State().lRz ) );

	/* process vib */
	procVib();
	}
}

void ImpNonJSImpl::SetEvents_State( const stINJSEnum::STATE state, ST_EVENT( EventHandle ) pfunc ) {
	m_curPack.pSTATE[state] = pfunc;
}

void ImpNonJSImpl::SetEvents_Button( const un32 index, ST_EVENT( EventNoParam ) pfunc ) {
	m_curPack.pBTN[index] = pfunc;
}

void ImpNonJSImpl::SetEvents_Stick( bool isLeft, ST_EVENT( EventStick ) pfunc ) {
	if ( isLeft ) {
		m_curPack.pLEFT = pfunc;
	} else {
		m_curPack.pRIGHT= pfunc;
	}
}

void ImpNonJSImpl::SetBehavior( const stInputEnum::BEHAVIOR behavior ) {
	switch ( behavior ) {
	case ST_BEHAVE_ANYTIME :
	m_isOnlyBehaveTop = false;
	break;
	case ST_BEHAVE_WHEN_TOPMOST :
	m_isOnlyBehaveTop = true;
	break;
	}
}

void ImpNonJSImpl::SetEvents_Pov( const un32 index, ST_EVENT( EventNoParam ) pfunc ) {
	m_curPack.pPOV[index] = pfunc;
}

void ImpNonJSImpl::AppTargetWindow( const wchar_t *className, const wchar_t *windowName ) {
	m_className.push_back( className );
	m_titles.push_back( windowName );
	//ref.push_back( windowName );
}

void ImpNonJSImpl::AppEventPack( const stEventPack & evtPack ) {
	m_packs.push_back( evtPack );
}

void ImpNonJSImpl::SetVibration( const un32 leftMotorSpeed, const un32 rightMotorSpeed, const un32 timeDuration, ST_EVENT( EventDoubleParam ) peffectFunc ) {
	m_vib.leftMotorSpeed	= leftMotorSpeed;
	m_vib.rightMotorSpeed	= rightMotorSpeed;
	m_vib.timeDuration		= timeDuration;
	m_vib.peffectFunc		= peffectFunc;

	m_stopwatch.Reset();
	m_stopwatch.Pause();
}
