#ifndef __IMPJS_INTERFACE_H__ 
#define __IMPJS_INTERFACE_H__ 

#include <core\Def.h>
#include <core\Event.h>

class stStrW;

ST_EVENT_START_IMPL
	ST_EVENT_DECLARE( EventNoParam, void, ( void ) )
	ST_EVENT_DECLARE( EventStick, void, ( un32 x, un32 y, un32 z ) )
ST_EVENT_END_IMPL

st_interface( stImpNonJS ) {
public:
	virtual un32 Loop() = 0;
	
	virtual void SetButtonEvents( const un32 index, ST_EVENT( EventNoParam ) pfunc ) = 0;
	virtual void SetPovEvents( const un32 index, ST_EVENT( EventNoParam ) pfunc ) = 0;
	virtual void SetStickEvents( bool isLeft, ST_EVENT( EventStick ) pfunc ) = 0;
	virtual HANDLE Reset( const stStrW &className, const stStrW &windowName ) = 0;
};

#endif /* !__IMPJS_INTERFACE_H__ */
