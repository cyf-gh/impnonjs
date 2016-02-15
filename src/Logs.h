#ifndef __IMPJS_LOGS_H__
#define __IMPJS_LOGS_H__

static bool isErrored = false;

#define ST_G_NOERRED isErrored == false

#define ST_G_SETERRED() isErrored = true
#define ST_G_SETNOERR() isErrored = false

#define ST_LOGONCE( log, is_err ) \
	if ( is_err != isErrored ) { \
		wprintf( log ); \
		isErrored = is_err; \
	} 

#define ST_INJS_LOGERR_INPUTLOST \
	L"[ERR]		Detected Joystick Device Lost.\n"

#define ST_INJS_LOGERR_NOTINITIALIZED \
	L"[ERR]		Detected Joystick Has Not Initialized Yet.\n"

#define ST_INJS_LOGERR_NOTACQUIRED \
	L"[ERR]		Detected Joystick Device Not Acquired.\n"

#define ST_INJS_LOGOK_GETSTATE \
	L"[ERR]		Device Runs Properly."

#define ST_INJS_LOGERR_WINDOWNOTFOUND \
	L"[ERR]		Assigned Window Not Found. ClassName : [%ls]  WindowName : [%ls]\n"

#define ST_INJS_LOGOK_WINDOWFOUND \
	L"[OK]		Assigned Window Found.     ClassName : [%ls]  WindowName : [%ls]\n"

#define ST_INJS_LOGOK_STARTPROCWIN \
	L"[OK]		Start Processing Window	   HWND :[0x%x]\n"

#define ST_INJS_LOGERR_LOSTWINDOW \
	L"\n[ERR]		Current Game Window Lost\n"

#define ST_INJS_LOGOK_INTERFACECREATE \
	L"[OK]		DirectInput Interface Created.\n"

#define ST_INJS_LOGOK_DEVICECREATE \
	L"[OK]		DirectInput Device Created.\n"

#define ST_INJS_LOGOK_DEVICENUMANDGUID \
	L"[OK]		Total [%d] Joystick(s).\n"

#endif /* !__IMPJS_LOGS_H__ */