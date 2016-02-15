#include <vector>
#include <core\Core.h>
#include <XInput.h>
using namespace stLibCore;

#include "..\include\Enum.h"
using namespace stInputEnum;
#include "..\include\ErrCode.h"
using namespace stInputErrCode;

#include "InputJS.h"
using namespace stInput;

#include "Logs.h"

std::vector<DIDEVICEINSTANCEW> g_ins;

stInputDev::stInputDev( HINSTANCE handle ) : m_pdevice( NULL ) {
	if ( FAILED( DirectInput8Create( handle, DIRECTINPUT_HEADER_VERSION, IID_IDirectInput8, ( void ** ) &m_pdevice, NULL) ) ) {  
		st_core_return( ST_ERR_CREATEINPUT );
	}
	wprintf( ST_INJS_LOGOK_INTERFACECREATE );
}

stInputDev::~stInputDev() {
	st_safe_rel( m_pdevice );
}

const DIJOYSTATE &stInputJS::State() {
	un32 err;

	while ( true ) {
		m_pdevice->Poll();
		m_pdevice->Acquire();
		if( SUCCEEDED( err = m_pdevice->GetDeviceState( sizeof( DIJOYSTATE ), &m_state ) ) ) { 
			st_core_return_with_var( ST_NOERR, m_state );		  
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


BOOL FAR PASCAL DIEnumDevicesCallback( LPCDIDEVICEINSTANCEW lpddi, LPVOID pvRef ) {
	g_ins.push_back( *lpddi );
	return DIENUM_CONTINUE;
}

stInputJS::stInputJS( HWND handle, un32 index, IDirectInput8 *pdevice, const un32 coopLevel ) : m_pdevice( NULL ) {
	un32 coopFlag = 0;

	st_zero_memory( &m_state, sizeof( m_state ) );

	pdevice->EnumDevices( DI8DEVCLASS_GAMECTRL, DIEnumDevicesCallback, NULL, 0 );
	wprintf( ST_INJS_LOGOK_DEVICENUMANDGUID, g_ins.size() );
	if ( FAILED( pdevice->CreateDevice( g_ins[index].guidInstance, &m_pdevice, NULL ) ) ) {
		st_core_return( ST_ERR_CREATEJS );
	}  
	wprintf( ST_INJS_LOGOK_DEVICECREATE );

	if ( FAILED( m_pdevice->SetDataFormat( &c_dfDIJoystick ) ) ) {
		st_core_return( ST_ERR_SETDATAFORMAT );		
	}

	switch ( coopLevel ) {
	case ST_DEVICE_OWN : {
	    coopFlag = DISCL_FOREGROUND | DISCL_EXCLUSIVE;  
		break;
	}
	case ST_DEVICE_SHARE: {
		coopFlag = DISCL_BACKGROUND | DISCL_NONEXCLUSIVE;
		break;
	}
	}
	
	if ( FAILED( m_pdevice->SetCooperativeLevel( handle, coopFlag ) ) ) {
		st_core_return( ST_ERR_SETCOOP );
	}  
}

stInputJS::~stInputJS() {
	m_pdevice->Unacquire();  
	st_safe_rel( m_pdevice );
}
