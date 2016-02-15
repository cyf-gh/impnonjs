#include "app-plugins.h"

int main() {
	printf( "impnonjsx86 Alpha 1\n\n" );
	stImpNonJS	*m_pjs;
	m_pjs = CreateImpNonJS();
	stAppPlugins app( m_pjs );
	m_pjs->Proc();
	return 0;
}