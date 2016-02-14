#ifndef __IMPJS_INTERFACE_H__ 
#define __IMPJS_INTERFACE_H__ 

#include <core\Def.h>
#include <core\Event.h>
#include "Enum.h"
#include "ErrCode.h"

class stStrW;

ST_EVENT_START_IMPL
	ST_EVENT_DECLARE( EventNoParam, void, ( void ) )
	ST_EVENT_DECLARE( EventStick, void, ( un32 x, un32 y, un32 z ) )
	ST_EVENT_DECLARE( EventDoubleParam, void, ( un32 &v1, un32 &v2 ) )
	ST_EVENT_DECLARE( EventHandle, void, ( HANDLE handle ) )
ST_EVENT_END_IMPL

st_interface( stImpNonJS ) {
public:
	virtual void		Proc( n32 &findWindowIndex ) = 0;
	virtual void		SetBehavior( const stInputEnum::BEHAVIOR behavior ) = 0;
	virtual void		SetEvents_Stick( bool isLeft, ST_EVENT( EventStick ) pfunc ) = 0;
	virtual void		SetEvents_State( const stINJSEnum::STATE state, ST_EVENT( EventHandle ) pfunc ) = 0;
	virtual void		SetEvents_Button( const un32 index,	ST_EVENT( EventNoParam ) pfunc ) = 0;
	virtual void		SetEvents_Pov( const un32 index, ST_EVENT( EventNoParam ) pfunc ) = 0;
	virtual void		AppTargetWindow( const wchar_t *className, const wchar_t *windowName ) = 0;
	virtual void		SetVibration( const un32 leftMotorSpeed, const un32 rightMotorSpeed, const un32 timeDuration, ST_EVENT( EventDoubleParam )peffectFunc ) = 0;
};

extern "C" stImpNonJS __declspec(dllimport)	*CreateImpNonJS();
extern "C" void		  __declspec(dllimport)	ReleaseImpNonJS( stImpNonJS **pinterface );

#endif /* !__IMPJS_INTERFACE_H__ */
