#ifndef __IMPJS_ENUM_H__
#define __IMPJS_ENUM_H__

#include <dinput.h>

namespace stInputEnum {

enum INPUT {
	ST_DEVICE_OWN		= 30,
	ST_DEVICE_SHARE		= 31
};

enum BEHAVIOR {
	ST_BEHAVE_WHEN_TOPMOST = 1,
	ST_BEHAVE_ANYTIME	   = 2
};

enum JOYSTICK {
	ST_JS_X				= DIJOFS_X,
	ST_JS_Y				= DIJOFS_Y,
	ST_JS_Z				= DIJOFS_Z,
	
	ST_JS_RX			= DIJOFS_RX,
	ST_JS_RY			= DIJOFS_RY,
	ST_JS_RZ			= DIJOFS_RZ,

	ST_JS_UP			= 0,
	ST_JS_DOWN			= 1,
	ST_JS_RIGHT			= 2,
	ST_JS_LEFT			= 3,

	ST_JS_BTN0			= 0,
	ST_JS_BTN1			= 1,
	ST_JS_BTN2			= 2,
	ST_JS_BTN3			= 3,
	ST_JS_BTN4			= 4,
	ST_JS_BTN5			= 5,
	ST_JS_BTN6			= 6,
	ST_JS_BTN7			= 7,
	ST_JS_BTN8			= 8,
	ST_JS_BTN9			= 9,
	ST_JS_BTN10			= 10,
	ST_JS_BTN11			= 11,
	ST_JS_BTN12			= 12,
	ST_JS_BTN13			= 13,
	ST_JS_BTN14			= 14,
	ST_JS_BTN15			= 15,
	ST_JS_BTN16			= 16,
	ST_JS_BTN17			= 17,
	ST_JS_BTN18			= 18,
	ST_JS_BTN19			= 19,
	ST_JS_BTN20			= 20,
	ST_JS_BTN21			= 21,
	ST_JS_BTN22			= 21,
	ST_JS_BTN23			= 23,
	ST_JS_BTN24			= 24,
	ST_JS_BTN25			= 25,
	ST_JS_BTN26			= 26,
	ST_JS_BTN27			= 27,
	ST_JS_BTN28			= 28,
	ST_JS_BTN29			= 29,
	ST_JS_BTN30			= 30,
	ST_JS_BTN31			= 31
};

}

namespace stINJSEnum {

enum STATE {
	ST_STATE_WINDOWFOUND,
	ST_STATE_WINDOWLOST,		// note that the vibration settings are invalid in this event.
	ST_MAX_STATE
};	

};

#endif /* !__IMPJS_ENUM_H__ */
