#include "..\CrossCircle\AcDbCrossCircle.h"

#pragma once

class CrossCircleJig : public AcEdJig
{
private:

	int mCurrentInputLevel ;

	AcDbDimDataPtrArray mDimData;

public:
	//- Array of input points, each level corresponds to the mCurrentInputLevel
	AcGePoint3dArray mInputPoints ;
	
	AcDbCrossCircle *mpCrCircle;

public:
	CrossCircleJig () ;
	~CrossCircleJig () ;

	AcEdJig::DragStatus startJig (AcDbCrossCircle *pEntityToJig);

protected:
	//- AcEdJig overrides
	//- input sampler
	virtual DragStatus sampler() ;

	//- jigged entity update
	virtual Adesk::Boolean update() ;

	//- jigged entity pointer return
	virtual AcDbEntity *entity () const ;

	//- dynamic dimension data setup
	virtual AcDbDimDataPtrArray *dimData (const double dimScale) ;

	//- dynamic dimension data update
	virtual Acad::ErrorStatus setDimValue (const AcDbDimData *pDimData, const double dimValue) ;

	//- Standard helper functions
	//- dynamic dimdata update function
	virtual Adesk::Boolean updateDimData() ;

	//- Std input to get a point with no rubber band
	AcEdJig::DragStatus GetStartPoint() ;

	//- Std input to get a point with rubber band from point
	AcEdJig::DragStatus GetNextPoint() ;
};
