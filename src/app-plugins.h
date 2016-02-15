#ifndef __IMPJS_APPPLUGINS_H__ 
#define __IMPJS_APPPLUGINS_H__ 

#include <base\Str.h>
#include <vector>
#include <impnonjs.h>
#include <Enum.h>
using namespace stINJSEnum;

class stAppPlugins {
private:
	std::vector<HINSTANCE>		m_dllHs;
	stImpNonJS					*m_pjs;
	void						loadPlugins();

public:
								stAppPlugins( stImpNonJS *pinterface );
								~stAppPlugins();
};

#endif /* !__IMPJS_APPPLUGINS_H__ */