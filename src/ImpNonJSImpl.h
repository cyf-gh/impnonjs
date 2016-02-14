#ifndef __IMPJS_INTERFACEIMPL_H__ 
#define __IMPJS_INTERFACEIMPL_H__ 

#include <vector>
#include <base\Str.h>
#include <utils\Stopwatch.h>
#include "..\include\impnonjs.h"

#include "InputJS.h"
using namespace stInput;


#define MAX_JS_BUTTON 8
#define MAX_JS_POV	  4

struct WindowInfo {
	stStrW				className;
	stStrW				title;
};

struct VibrationInfo {
	un32				leftMotorSpeed;
	un32				rightMotorSpeed;
	f64					timeDuration;
	ST_EVENT( EventDoubleParam )
						peffectFunc;
};

class ImpNonJSImpl : public stImpNonJS {

	st_class_no_bin_cpy( ImpNonJSImpl )

private:
	std::vector<WindowInfo>	m_windowInfos;
	stLibUtils::stStopWatch m_stopwatch;
	stInputDev *		m_pdev;
	stInputJS *			m_pJS;
	VibrationInfo		m_vib;
	HANDLE				m_curActiveWindow;
	bool				m_isOnlyBehaveTop;

	bool				checkWindowExist( n32 &findWindowIndex );
	void				resetVib();
	void				procVib();

public: 
	void				Proc( n32 &findWindowIndex );
	
	void				SetEvents_State( const stINJSEnum::STATE state, ST_EVENT( EventHandle ) pfunc );
	void				SetEvents_Button( const un32 index, ST_EVENT( EventNoParam ) pfunc );
	void				SetEvents_Pov( const un32 index, ST_EVENT( EventNoParam ) pfunc );
	void				SetEvents_Stick( bool isLeft, ST_EVENT( EventStick ) pfunc );
	void				SetBehavior( const stInputEnum::BEHAVIOR behavior );
	void				AppTargetWindow( const wchar_t *className, const wchar_t *windowName );
	void				SetVibration( const un32 leftMotorSpeed, const un32 rightMotorSpeed, const un32 timeDuration, ST_EVENT( EventDoubleParam )peffectFunc );
						
						ImpNonJSImpl();
						~ImpNonJSImpl();
};

#endif /* !__IMPJS_INTERFACEIMPL_H__  */