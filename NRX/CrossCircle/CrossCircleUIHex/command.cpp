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
		acutPrintf(L"\nНе указан Entity");
	}

	return pEnt;
}


//igorab запрос на селекцию существующего объекта AcDbCrossCircle
int CrCircleSelect()
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

	//центр оси вращения (переносим ось координат)
	double C0_x = center[0].x + dCx;
	double C0_y = center[0].y - dCy;

	AcGePoint2d PO(-dCx, dCy);

	c_center[0].x = PO.x;
	c_center[0].y = PO.y;

	//вращение вокруг оси
	for (int i = 1; i < 6; i ++)
	{
		c_center[i].x = PO.x * cos(i * PI / 3) + PO.y * sin(i * PI / 3);
		c_center[i].y = -PO.x * sin(i* PI / 3) + PO.y * cos(i* PI / 3);
 	}

	//перенос оси координат назад
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
		acutPrintf(L"\nНевозможно открыть таблицу блоков");
		return (RTNORM);
	}

	AcDbBlockTableRecord* pBlockTableRecord = NULL;

	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

	if (pBlockTableRecord == NULL)
	{
		acutPrintf(L"\nНет доступа к блоку *Model_space ");
		return (RTNORM);
	}

	for (int i = 1; i < 6; i++)
	{
		es = pBlockTableRecord->appendAcDbEntity(newpCrCircle[i]);

		if (es != Acad::eOk)
		{
			acutPrintf(L"\nОшибка добавления к БД объекта");
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