#ifndef __IMPJS_APPEVENTS_H__ 
#define __IMPJS_APPEVENTS_H__ 

#include <core\Def.h>
#include <core\Event.h>

#define ST_MKKB_EVENT( func_name, vk )

ST_EVENT_START_IMPL
	ST_EVENT_DECLARE( EventKeyBoard, void, ( void ) )
	ST_EVENT_DECLARE( EventKeyBoardComb, void, ( un8 vk, un8 scan, un32 flags, un32 exInfo, ST_EVENT( EventKeyBoard ) ) )
ST_EVENT_END_IMPL

static void Evt_Enter() {
	keybd_event( VK_RETURN, 0 , 0, 0 );
	keybd_event( VK_RETURN, 0 , KEYEVENTF_KEYUP, 0 );	
}

#endif /* !__IMPJS_APPEVENTS_H__ */