//----- acrxEntryPoint.cpp

#include "StdAfx.h"
#include "resource.h"
#include "command.h"

#ifdef NCAD
#define CROSSCIRCLE_DBX_NAME _T("CrossCircle.nrx")
#else
#define CROSSCIRCLE_DBX_NAME _T("CrossCircle.dbx")
#endif

//----- EntryPoint
class CCrossCircleUIHexApp : public AcRxArxApp
{

	public:
	CCrossCircleUIHexApp () : AcRxArxApp ()
	{
	}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt)
	{
		if (!acrxLoadModule(CROSSCIRCLE_DBX_NAME, 0))
		  return AcRx::kRetError;

		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;

		acrxUnlockApplication(pkt);		
		acrxDynamicLinker->registerAppMDIAware(pkt);

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt)
	{
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		acrxUnloadModule(CROSSCIRCLE_DBX_NAME);

		return (retCode) ;
	}

	virtual void RegisterServerComponents ()
	{
	}

	//igorab - CrossCircleUIHex.CrCircleHex command 
	static void CrossCircleUIHexCrCircleHex(void)
	{
		CrCircleSelect();

		CrCircleConnect();
	}
};

IMPLEMENT_ARX_ENTRYPOINT(CCrossCircleUIHexApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CCrossCircleUIHexApp, CrossCircleUIHex, CrCircleHex, Коннектор, ACRX_CMD_MODAL, NULL)
