#include "StdAfx.h"
#include "command.h"
#include "CrossCircleJig.h"
#include "CrossCircleJigConnect.h"
#include "../CrossCircle/AcDbCrossCircleConnector.h"

// Построение круга
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
	AcDbCrossCircleConnector *pCrCircleConnect = new AcDbCrossCircleConnector();

	CrossCircleJigConnect * pCrCircleJigConnect = new CrossCircleJigConnect();

	pCrCircleJigConnect->startJig(pCrCircleConnect);

	delete pCrCircleJigConnect;

}