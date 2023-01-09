#include "StdAfx.h"
#include "command.h"
#include "CrossCircleJig.h"
#include "CrossCircleJigConnect.h"
#include "../CrossCircle/AcDbCrossCircleConnector.h"

AcDbEntity* selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode)
{
	ads_name en;
	ads_point pt;
	acedEntSel(_RXST("\nSelect an entity: "), en, pt);
	// Exchange the ads_name for an object ID.
	//
	acdbGetObjectId(eId, en);
	AcDbEntity* pEnt;
	acdbOpenObject(pEnt, eId, openMode);
	return pEnt;
}


// запрос на селекцию существующего объекта AcDbCrossCircle
void CrCircleSelect()
{
	AcDbObjectId objId;

	//AcDbCrossCircle *pCrCircle = new AcDbCrossCircle();

	AcDbEntity * pCrCircle = selectEntity(objId, AcDb::kForWrite);

	AcDbCrossCircle* newCrCircle = new AcDbCrossCircle();

	pCrCircle->copied(pCrCircle, newCrCircle);

	AcDbBlockTable* pBlockTable;
	acdbHostApplicationServices()->workingDatabase() ->getSymbolTable(pBlockTable, AcDb::kForRead);
	AcDbBlockTableRecord* pBlockTableRecord;

	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	pBlockTable->close();

	AcDbObjectId lineId;
	pBlockTableRecord->appendAcDbEntity(lineId, newCrCircle);
	pBlockTableRecord->close();

	//CrossCircleJig *pCrCicleJig = new CrossCircleJig();
	//pCrCicleJig->startJig(newCrCircle);
	//delete pCrCicleJig;

	
}

//igorab
void CrCircleConnect()
{
	AcDbCrossCircleConnector *pCrCircleConnect = new AcDbCrossCircleConnector();

	CrossCircleJigConnect * pCrCircleJigConnect = new CrossCircleJigConnect();

	pCrCircleJigConnect->startJig(pCrCircleConnect);

	delete pCrCircleJigConnect;

}