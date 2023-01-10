#include "StdAfx.h"
#include "command.h"
#include "..\CrossCircle\AcDbCrossCircle.h"
#include "CrossCircleJigConnect.h"
#include "../CrossCircle/AcDbCrossCircleConnector.h"

AcDbEntity* selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode)
{
	Acad::ErrorStatus es;

	ads_name en;
	ads_point pt;
	acedEntSel(_RXST("\nSelect an entity: "), en, pt);
	// Exchange the ads_name for an object ID.
	//
	es = acdbGetObjectId(eId, en);

	AcDbEntity* pEnt = NULL;

	if (es == Acad::eOk)
	{
		es = acdbOpenObject(pEnt, eId, openMode);
	}

	if (es != Acad::eOk)
	{
		acutPrintf(L"\n�� ������ Entity");
	}

	return pEnt;
}


//igorab ������ �� �������� ������������� ������� AcDbCrossCircle
int CrCircleSelect()
{
	Acad::ErrorStatus es;
	AcDbObjectId objId;

	AcDbCrossCircle* pCrCircle = (AcDbCrossCircle*)selectEntity(objId, AcDb::kForWrite);

	AcGePoint3d center[6];
	center[0] = pCrCircle->center();
	double radius = pCrCircle->radius();
	double angle = 0;

	AcDbCrossCircle* newpCrCircle[6];

	newpCrCircle[0] = pCrCircle;

	for (int i = 1; i < 6; i++)
	{
		newpCrCircle[i] = new AcDbCrossCircle();
		pCrCircle->copied(pCrCircle, newpCrCircle[i]);

		newpCrCircle[i]->setRadius(radius);
		center[i] = AcGePoint3d(center[0].x +i*2*radius, center[0].y, 0);
		newpCrCircle[i]->setCenter(center[i]);
		newpCrCircle[i]->setAngle(angle + i*PI/3);
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
			acutPrintf(L"\n������ ���������� � �� �������");
			return (RTNORM);
		}
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
	AcDbCrossCircleConnector *pCrCircleConnect = new AcDbCrossCircleConnector();

	CrossCircleJigConnect * pCrCircleJigConnect = new CrossCircleJigConnect();

	pCrCircleJigConnect->startJig(pCrCircleConnect);

	delete pCrCircleJigConnect;

}