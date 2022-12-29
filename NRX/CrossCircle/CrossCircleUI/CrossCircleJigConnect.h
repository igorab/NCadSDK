//-----------------------------------------------------------------------------
//- CrossCircleJig.h
#include "..\CrossCircle\AcDbCrossCircle.h"

#pragma once

//----igorab-----------
class CrossCircleJigConnect : public AcEdJig
{
  int mCurrentInputLevel ;
  
  AcDbDimDataPtrArray mDimData ;

public:
  
  AcGePoint3dArray mInputPoints ;

  AcDbCrossCircle * mpCrCircle; // Entity being jigged

  CrossCircleJigConnect();

  ~ CrossCircleJigConnect();

  AcEdJig::DragStatus startJig (AcDbCrossCircle *pEntityToJig) ;

protected:
  
  virtual DragStatus sampler () ;
  
  virtual Adesk::Boolean update () ;
  
  virtual AcDbEntity *entity () const ;

  virtual AcDbDimDataPtrArray *dimData (const double dimScale) ;
  
  virtual Acad::ErrorStatus setDimValue (const AcDbDimData *pDimData, const double dimValue) ;

  virtual Adesk::Boolean updateDimData () ;

  AcEdJig::DragStatus GetStartPoint () ;
  
  AcEdJig::DragStatus GetNextPoint () ;
} ;
