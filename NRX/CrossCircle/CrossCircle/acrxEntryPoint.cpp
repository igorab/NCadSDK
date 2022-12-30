#include "StdAfx.h"
#include "resource.h"
#include "AcDbCrossCircle.h"

// EntryPoint
class CCrossCircleApp: public AcRxDbxApp
{
public:
	CCrossCircleApp(): AcRxDbxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt)
	{
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kInitAppMsg(pkt);
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt)
	{
		AcRx::AppRetCode retCode =AcRxDbxApp::On_kUnloadAppMsg(pkt);
		return (retCode) ;
	}

	virtual void RegisterServerComponents ()
	{
		if (FAILED(::DllRegisterServer()))
			acutPrintf (_RXST("CrossCircle: Cannot self-register COM server, HKLM is not writable.\n")) ;
	}
} ;

IMPLEMENT_ARX_ENTRYPOINT(CCrossCircleApp)
