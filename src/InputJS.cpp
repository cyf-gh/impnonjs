#include <core\Core.h>
using namespace stLibCore;

#include "..\include\Enum.h"
using namespace stInputEnum;
#include "..\include\ErrCode.h"
using namespace stInputErrCode;

#include "InputJS.h"
using namespace stInput;

#include "Logs.h"

stInputDev::stInputDev( HINSTANCE handle ) : m_pdevice( NULL ) {
	if ( FAILED( DirectInput8Create( handle, DIRECTINPUT_HEADER_VERSION, IID_IDirectInput8, ( void ** ) &m_pdevice, NULL) ) ) {  
		st_core_return( ST_ERR_CREATEINPUT );
	}
}

stInputDev::~stInputDev() {
	st_safe_rel( m_pdevice );
}

const DIJOYSTATE &stInputJS::State() {
	un32 err;

	while ( true ) {
		m_pJS->Poll();
		m_pJS->Acquire();
		if( SUCCEEDED( err = m_pJS->GetDeviceState( sizeof( m_state ), &m_state ) ) ) { 
			break;  
		}
		switch ( err ) {
		case DIERR_NOTINITIALIZED : {
			ST_LOGONCE( ST_INJS_LOGERR_NOTINITIALIZED, true );
			break;
		}
		case DIERR_INPUTLOST : {
			ST_LOGONCE( ST_INJS_LOGERR_INPUTLOST, true );
			st_core_return_with_var( ST_ERR_DEVICELOST, m_state );
			break;
		}
		case DIERR_NOTACQUIRED: {
			ST_LOGONCE( ST_INJS_LOGERR_NOTACQUIRED, true );
			st_core_return_with_var( ST_ERR_NOTACQUIRED, m_state );
			break;
		}
		}
	}
	ST_LOGONCE( ST_INJS_LOGOK_GETSTATE, false );
	st_core_return_with_var( ST_NOERR, m_state );
}

stInputJS::stInputJS( HWND handle, IDirectInput8 *pdevice, const un32 coopLevel ) : m_pJS( NULL ) {
	un32 coopFlag = 0;

	st_zero_memory( &m_state, sizeof( m_state ) );

	if ( FAILED( pdevice->CreateDevice( GUID_Joystick, &m_pJS, NULL ) ) ) {
		st_core_return( ST_ERR_CREATEJS );
	}  
	m_pJS->SetDataFormat( &c_dfDIJoystick );
	
	switch ( coopLevel ) {
	case ST_DEVICE_OWN : {
	    coopFlag =  DISCL_BACKGROUND | DISCL_NONEXCLUSIVE;  
		break;
	}
	case ST_DEVICE_SHARE: {
		coopFlag =  DISCL_FOREGROUND | DISCL_EXCLUSIVE;
		break;
	}
	}
	
	if ( FAILED( m_pJS->SetCooperativeLevel( handle, coopFlag ) ) ) {
		st_core_return( ST_ERR_SETCOOP );
	}  
}

stInputJS::~stInputJS() {
	m_pJS->Unacquire();  
	st_safe_rel( m_pJS );
}
