#include "..\CrossCircle\AcDbCrossCircleConnector.h"
#pragma once

//----igorab-----------
class CrossCircleJigConnect : public AcEdJig
{
	int  mCurrentInputLevel;
	AcDbDimDataPtrArray  mDimData;

public:
  
	AcGePoint3dArray mInputPoints ;

	AcDbCrossCircleConnector * mpCrCircle; 

	CrossCircleJigConnect();

	~CrossCircleJigConnect();

	AcEdJig::DragStatus startJig(AcDbCrossCircleConnector *pEntityToJig) ;

protected:
  
	virtual DragStatus sampler();

	virtual Adesk::Boolean update();

	virtual AcDbEntity *entity() const;

	virtual AcDbDimDataPtrArray *dimData(const double dimScale);

	virtual Acad::ErrorStatus setDimValue(const AcDbDimData *pDimData, const double dimValue);

	virtual Adesk::Boolean updateDimData();

	AcEdJig::DragStatus GetStartPoint();

	AcEdJig::DragStatus GetNextPoint();
} ;
