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
//- MgCrossCircle.cpp : Initialization functions
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"

HINSTANCE _hdllInstance ;

//-----------------------------------------------------------------------------
//- DLL Entry Point
#pragma managed(push, off)
extern "C"
BOOL WINAPI DllMain (HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
  //- Remove this if you use lpReserved
  UNREFERENCED_PARAMETER(lpReserved) ;

  if ( dwReason == DLL_PROCESS_ATTACH ) {
    _hdllInstance =hInstance ;
  } else if ( dwReason == DLL_PROCESS_DETACH ) {
  }
  return (TRUE) ;
}
#pragma managed(pop)

