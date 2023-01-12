#include "StdAfx.h"
#include "command.h"
#include "../CrossCircle/AcDbCrossCircle.h"
#include "CrossCircleJigConnect.h"
#include "../CrossCircle/AcDbCrossCircleConnector.h"

AcDbEntity* selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode)
{
	Acad::ErrorStatus es;

	ads_name en;
	ads_point pt;
	acedEntSel(_RXST("\n������� ������ ���� CrossCircle: "), en, pt);
	
	es = acdbGetObjectId(eId, en);

	AcDbEntity* pEnt = NULL;

	if (es == Acad::eOk)
	{
		es = acdbOpenObject(pEnt, eId, openMode);
	}

	if (es != Acad::eOk)
	{
		acutPrintf(L"\n�� ������ ������ CrossCircle");
	}

	return pEnt;
}


//igorab ������ �� �������� ������������� ������� AcDbCrossCircle
int CrCircleSelectAndConnect()
{
	Acad::ErrorStatus es;
	AcDbObjectId objId;

	AcDbCrossCircle* pCrCircle = (AcDbCrossCircle*)selectEntity(objId, AcDb::kForWrite);

	AcGePoint3d center[6];
	center[0] = pCrCircle->center();
	double radius = pCrCircle->radius();
	double angle = pCrCircle->angle();

	AcGePoint2d c_center[6];
	double dCx = 2*radius * sin(angle);
	double dCy = 2*radius * cos(angle);

	//����� ��� �������� 
	double C0_x = center[0].x + dCx;
	double C0_y = center[0].y - dCy;

	AcGePoint2d PO(-dCx, dCy);

	double R_C0 = PO.asVector().length();

	c_center[0].x = PO.x;
	c_center[0].y = PO.y;

	//�������� ������ ���
	for (int i = 1; i < 6; i ++)
	{
		c_center[i].x = PO.x * cos(i * PI / 3) + PO.y * sin(i * PI / 3);
		c_center[i].y = -PO.x * sin(i* PI / 3) + PO.y * cos(i* PI / 3);
 	}

	//������� ��� ��������� �����
	for (int i = 0; i < 6; i++)
	{
		center[i].x = C0_x + c_center[i].x;
		center[i].y = C0_y + c_center[i].y;
		center[i].z = 0;
	}

	AcDbCrossCircle* newpCrCircle[6];
	newpCrCircle[0] = pCrCircle;

	for (int i = 1; i < 6; i++)
	{
		newpCrCircle[i] = new AcDbCrossCircle();
		pCrCircle->copied(pCrCircle, newpCrCircle[i]);

		newpCrCircle[i]->setRadius(radius);
		newpCrCircle[i]->setCenter(center[i]);
		newpCrCircle[i]->setAngle(angle - i*PI/3);
	}

	AcDbBlockTable* pBlockTable = NULL;

	es = acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::kForRead);

	if (es != Acad::eOk)
	{
		acutPrintf(L"\n���������� ������� ������� ������");
		return (RTNORM);
	}

	AcDbBlockTableRecord* pBlockTableRecord = NULL;

	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

	if (pBlockTableRecord == NULL)
	{
		acutPrintf(L"\n��� ������� � ����� *Model_space ");
		return (RTNORM);
	}

	for (int i = 1; i < 6; i++)
	{
		es = pBlockTableRecord->appendAcDbEntity(newpCrCircle[i]);

		if (es != Acad::eOk)
		{
			acutPrintf(L"\n������ ���������� � �� ������� AcDbCrossCircle");
			return (RTNORM);
		}
	}

	AcDbCrossCircleConnector* pCrCircleConnect = new AcDbCrossCircleConnector();
	AcGePoint3d centerConnect(C0_x, C0_y, 0);
	const AcGeVector3d normal = AcGeVector3d::kZAxis;
	pCrCircleConnect->setNormal(normal);
	pCrCircleConnect->setCenter(centerConnect);
	AcGeVector3d vRadius(radius/4, 0, 0);
	vRadius = vRadius.rotateBy(angle + PI/6, normal);
	pCrCircleConnect->setVecRadius(vRadius);
	double rayLngth = R_C0 - 1.2*radius - pCrCircleConnect->radius();
	pCrCircleConnect->setLength(rayLngth);

	es = pBlockTableRecord->appendAcDbEntity(pCrCircleConnect);

	if (es != Acad::eOk)
	{
		acutPrintf(L"\n������ ���������� � �� ������� AcDbCrossCircleConnector");
		return (RTNORM);
	}


	for (int i = 1; i < 6; i++)
		newpCrCircle[i]->close();

	pBlockTableRecord->close();
	pBlockTable->close();

	return (RTNORM);
}

//igorab 
void CrCircleConnect()
{
	/*
	 AcDbCrossCircleConnector *pCrCircleConnect = new AcDbCrossCircleConnector();

	CrossCircleJigConnect * pCrCircleJigConnect = new CrossCircleJigConnect();

	pCrCircleJigConnect->startJig(pCrCircleConnect);

	delete pCrCircleJigConnect;
	*/

}