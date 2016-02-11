#ifndef __IMPJS_INTERFACEIMPL_H__ 
#define __IMPJS_INTERFACEIMPL_H__ 

#include "..\include\impnonjs.h"

class stStrW;

#include "InputJS.h"
using namespace stInput;


#define MAX_JS_BUTTON 8
#define MAX_JS_POV	  4

class ImpNonJSImpl : public stImpNonJS {
private:
	stInputDev *		m_pdev;
	stInputJS *			m_pJS;
	HANDLE				m_window;

public: 
	un32				Loop();
	
	void				SetButtonEvents( const un32 index, ST_EVENT( EventNoParam ) pfunc );
	void				SetPovEvents( const un32 index, ST_EVENT( EventNoParam ) pfunc );
	void				SetStickEvents( bool isLeft, ST_EVENT( EventStick ) pfunc );

	HANDLE				Reset( const stStrW &className, const stStrW &windowName );
						ImpNonJSImpl();
						~ImpNonJSImpl();
};

#endif /* !__IMPJS_INTERFACEIMPL_H__  */