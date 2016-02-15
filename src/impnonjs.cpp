#include <core\Core.h>
using namespace stLibCore;

#include "ImpNonJSImpl.h"
#include "InputJS.h"
#include "..\include\impnonjs.h"

extern "C" stImpNonJS __declspec(dllexport) *CreateImpNonJS() {
	return new ImpNonJSImpl;
}
	
extern "C" void __declspec(dllexport) ReleaseImpNonJS( stImpNonJS **pinterface ) {
	delete pinterface;
}