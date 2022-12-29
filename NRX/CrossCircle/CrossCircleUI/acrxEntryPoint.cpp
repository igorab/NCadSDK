//
// �������� (�) 2019, ��� ��������� ����������. ��� ����� ��������.
// 
// ������ ����������� �����������, ��� �������������� ����� �� ����, ���
// ������������ � ���������������� ��������� ����������� ��� ��������� ����������.
// ������ ����������� ����������� ����� �������������� ��� ���������� � �������
// � ������ ������������� ����������� ��������� ��� ���������� �������
// �������������, ����������� � ������������� �������� �������������
// �� ������������� ��������� ��� ��� ���������� nanoCAD��.
// 
// ������ ����������� ����������� �������� � ������������ � �����������������
// ���������� ��������� �� ���������������� ������������� � ��������������
// ��������� ������.
// 
// ��������� ������ ����������� �����������,  ��� ������������ �
// ���������������� ��������� �� ������������ � ��������� �������������,
// ���������� ����. 
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "command.h"

#ifdef NCAD
#define CROSSCIRCLE_DBX_NAME _T("CrossCircle.nrx")
#else
#define CROSSCIRCLE_DBX_NAME _T("CrossCircle.dbx")
#endif

//----- EntryPoint
class CCrossCircleUIApp : public AcRxArxApp
{

public:
  CCrossCircleUIApp () : AcRxArxApp ()
  {
  }

  virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt)
  {

    if (!acrxLoadModule(CROSSCIRCLE_DBX_NAME, 0))
      return AcRx::kRetError;


    // You *must* call On_kInitAppMsg here
    AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;

    acrxUnlockApplication(pkt);		// Try to allow unloading
    acrxDynamicLinker->registerAppMDIAware(pkt);

    return (retCode) ;
  }

  virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt)
  {
    // You *must* call On_kUnloadAppMsg here
    AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

    acrxUnloadModule(CROSSCIRCLE_DBX_NAME);

    return (retCode) ;
  }

  virtual void RegisterServerComponents ()
  {
  }

  // - CrossCircleUI.CrCircle command (do not rename)
  static void CrossCircleUICrCircle(void)
  {
  	// CrCircle(); igorab

      CrCircleConnect(); // igorab
  }

	// igorab
  static void CrossCircleUICrCircleConnect()
  {
      CrCircleConnect();
  }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCrossCircleUIApp)


ACED_ARXCOMMAND_ENTRY_AUTO(CCrossCircleUIApp, CrossCircleUI, CrCircle, ���������, ACRX_CMD_MODAL, NULL)
