#ifndef __IMPJS_INPUTJS_H__
#define __IMPJS_INPUTJS_H__ 

#include <core\Def.h>
#include "dinput.h"

#pragma comment( lib, "dinput8.lib" )
#pragma comment( lib, "dxguid.lib" )

namespace stInput {

class stInputDev {

	st_class_no_bin_cpy( stInputDev )

private:
	IDirectInput8 *			m_pdevice; 

public:
	void *					Handle() const { return m_pdevice; };
							stInputDev( HINSTANCE handle );
							~stInputDev();
};

class stInputJS {

	st_class_no_bin_cpy( stInputJS )


private:
	IDirectInputDevice8 *	m_pdevice;
	std::vector<DIDEVICEINSTANCEW> m_instances;
	DIJOYSTATE				m_state;	

public: 
	const DIJOYSTATE &		State();
							stInputJS( HWND handle, un32 index, IDirectInput8 *pdevice, const un32 coopLevel );
							~stInputJS();
};

} /* stInput */

#endif /* !__IMPJS_INPUTJS_H__ */
