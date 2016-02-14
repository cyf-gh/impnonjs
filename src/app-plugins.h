#ifndef __IMPJS_APPPLUGINS_H__ 
#define __IMPJS_APPPLUGINS_H__ 

#include <base\Str.h>
#include <vector>
#include <impnonjs.h>
#include <Enum.h>
using namespace stINJSEnum;

ST_EVENT_START_IMPL
	ST_EVENT_DECLARE( EventGetWStr, void, ( wchar_t **ppstrw ) )
	ST_EVENT_DECLARE( EventPassInstance, void, ( void *pinstance ) )
	ST_EVENT_DECLARE( EventBehavior, void, ( un32 &behavior ) )
ST_EVENT_END_IMPL

class stEventPack {
public:
	ST_EVENT( EventNoParam )	pBTN[8];
	ST_EVENT( EventNoParam )	pPOV[4];
	ST_EVENT( EventHandle)		pSTATE[ST_MAX_STATE];
	ST_EVENT( EventStick )		pLEFT;
	ST_EVENT( EventStick )		pRIGHT;

	ST_EVENT( EventBehavior )		p_Behavior;
	ST_EVENT( EventPassInstance )	p_PassInstance;
	ST_EVENT( EventGetWStr )		p_ClassName;
	ST_EVENT( EventGetWStr )		p_TitleName;

public:
								stEventPack();
};

class stAppPlugins {
private:
	stImpNonJS					*m_pjs;
	std::vector<stEventPack>	m_packs;
	std::vector<HINSTANCE>		m_dllHs;
	void						loadPlugins();

public:
	un32						Loop();
								stAppPlugins();
								~stAppPlugins();
};

#endif /* !__IMPJS_APPPLUGINS_H__ */