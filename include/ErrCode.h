#ifndef __IMPJS_ERRCODE_H__
#define __IMPJS_ERRCODE_H__

namespace stInputErrCode {

enum INPUTDEVICE {
	ST_ERR_CREATEINPUT		= -600
};

enum INPUTJS {
	ST_ERR_CREATEJS			= -640,
	ST_ERR_SETCOOP			= -641,
	ST_ERR_DEVICELOST		= -642,
	ST_ERR_NOTACQUIRED		= -643
};

}


#endif /* !__IMPJS_ERRCODE_H__ */

