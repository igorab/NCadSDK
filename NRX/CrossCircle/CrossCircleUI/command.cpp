#include "StdAfx.h"
#include "command.h"
#include "CrossCircleJig.h"

// Построение круга
void CrCircle ()
{
	AcDbCrossCircle *pCrCircle = new AcDbCrossCircle();
	CrossCircleJig *pCrCicleJig = new CrossCircleJig();

	pCrCicleJig->startJig(pCrCircle);
	delete pCrCicleJig;
}
