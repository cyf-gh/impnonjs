#include <core\Core.h>
using namespace stLibCore;

#include "ImpNonJSImpl.h"
#include "InputJS.h"
#include "..\include\impnonjs.h"

extern "C" stImpNonJS __declspec(dllexport) *CreateImpNonJS() {
	return st_new_class<ImpNonJSImpl>( ImpNonJSImpl() ) ;
}
	
extern "C" void __declspec(dllexport) ReleaseImpNonJS( stImpNonJS **pinterface ) {
	st_delete_class<ImpNonJSImpl>( ( ImpNonJSImpl ** )pinterface );
}