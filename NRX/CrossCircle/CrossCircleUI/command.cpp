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

#include "StdAfx.h"
#include "command.h"
#include "CrossCircleJig.h"

// ���������� �����
void CrCircle ()
{
  AcDbCrossCircle *pCrCircle = new AcDbCrossCircle();
  CrossCircleJig *pCrCicleJig = new CrossCircleJig();

  pCrCicleJig->startJig(pCrCircle);
  delete pCrCicleJig;

}

//igorab
void CrCircleConnect()
{
	AcDbCrossCircle *pCrCircleConnect = new AcDbCrossCircle();

	CrossCircleJig * pCrCircleJigConnect = new CrossCircleJig();

	pCrCircleJigConnect->startJig(pCrCircleConnect);

	delete pCrCircleJigConnect;

}