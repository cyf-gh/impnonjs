#ifndef __IMPJS_INTERFACE_H__ 
#define __IMPJS_INTERFACE_H__ 

#include <core\Def.h>
#include <core\Event.h>
#include "Enum.h"
#include "ErrCode.h"

class stStrW;
class stEventPack;

ST_EVENT_START_IMPL
	ST_EVENT_DECLARE( EventNoParam, void, ( void ) )
	ST_EVENT_DECLARE( EventStick, void, ( un32 x, un32 y, un32 z ) )
	ST_EVENT_DECLARE( EventDoubleParam, void, ( un32 &v1, un32 &v2 ) )
	ST_EVENT_DECLARE( EventHandle, void, ( HANDLE handle ) )
	ST_EVENT_DECLARE( EventGetWStr, void, ( wchar_t **ppstrw ) )
	ST_EVENT_DECLARE( EventPassInstance, void, ( void *pinstance ) )
	ST_EVENT_DECLARE( EventBehavior, void, ( un32 &behavior ) )
ST_EVENT_END_IMPL

st_interface( stImpNonJS ) {
public:
	virtual void		Proc() = 0;
	virtual void		SetBehavior( const stInputEnum::BEHAVIOR behavior ) = 0;
	virtual void		SetEvents_Stick( bool isLeft, ST_EVENT( EventStick ) pfunc ) = 0;
	virtual void		SetEvents_State( const stINJSEnum::STATE state, ST_EVENT( EventHandle ) pfunc ) = 0;
	virtual void		SetEvents_Button( const un32 index,	ST_EVENT( EventNoParam ) pfunc ) = 0;
	virtual void		SetEvents_Pov( const un32 index, ST_EVENT( EventNoParam ) pfunc ) = 0;
	virtual void		AppTargetWindow( const wchar_t *className, const wchar_t *windowName ) = 0;
	virtual void		AppEventPack( const stEventPack &evtPack ) = 0;
	virtual void		SetVibration( const un32 leftMotorSpeed, const un32 rightMotorSpeed, const un32 timeDuration, ST_EVENT( EventDoubleParam )peffectFunc ) = 0;
};

class stEventPack {
public:
	ST_EVENT( EventNoParam )	pBTN[8];
	ST_EVENT( EventNoParam )	pPOV[4];
	ST_EVENT( EventHandle)		pSTATE[stINJSEnum::ST_MAX_STATE];
	ST_EVENT( EventStick )		pLEFT;
	ST_EVENT( EventStick )		pRIGHT;

	ST_EVENT( EventBehavior )		p_Behavior;
	ST_EVENT( EventPassInstance )	p_PassInstance;
	ST_EVENT( EventGetWStr )		p_ClassName;
	ST_EVENT( EventGetWStr )		p_TitleName;

public:
								stEventPack();
};

ST_INLINE stEventPack::stEventPack() : p_PassInstance( NULL ), pLEFT( NULL ), pRIGHT( NULL ), p_ClassName( NULL ), p_TitleName( NULL ), p_Behavior( NULL ) {
	for ( un32 i = 0; i < 8; ++i ) {
		pBTN[i] = NULL;
	}
	for ( un32 i = 0; i < 4; ++i ) {
		pPOV[i] = NULL;
	}
	for ( un32 i = 0; i < stINJSEnum::ST_MAX_STATE; ++i ) {
		pSTATE[i] = NULL;
	}
}

extern "C" stImpNonJS __declspec(dllimport)	*CreateImpNonJS();
extern "C" void		  __declspec(dllimport)	ReleaseImpNonJS( stImpNonJS **pinterface );

#endif /* !__IMPJS_INTERFACE_H__ */
