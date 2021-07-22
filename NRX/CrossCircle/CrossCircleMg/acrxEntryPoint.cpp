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

#ifdef NCAD
#define CROSSCIRCLE_DBX_NAME L"CrossCircle.nrx"
#else
#define CROSSCIRCLE_DBX_NAME L"CrossCircle.dbx"
#endif

static AcMgObjectFactoryBase **g_CrCircles = NULL;

/////////////////////////////////////////////////////////////////////
// EntryPoint
extern "C" __declspec( dllexport ) AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
  switch (msg)
  {
  case AcRx::kInitAppMsg:
    {
      if (!acrxLoadModule(CROSSCIRCLE_DBX_NAME, 0))
        return AcRx::kRetError;

      acrxDynamicLinker->registerAppMDIAware(pkt);
      // create a new object factory array
      static AcMgObjectFactoryBase* CrCircles[] = 
      {
        new AcMgObjectFactory<HostMgd::Samples::MgCrCrc,AcDbCrossCircle>(), 
        // end the array with a NULL
        NULL
      };

      g_CrCircles = CrCircles;
    }
    break;
  case AcRx::kPreQuitMsg:
    {
      // clean up
      int i=0;
      while (g_CrCircles[i]!=NULL)
        delete g_CrCircles[i++];
    }
    break;
  case AcRx::kUnloadAppMsg:
    acrxUnloadModule(CROSSCIRCLE_DBX_NAME);
    break;
  }
  return AcRx::kRetOK;
}
