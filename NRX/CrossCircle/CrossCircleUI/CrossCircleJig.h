//
// Копирайт (С) 2019, ООО «Нанософт разработка». Все права защищены.
// 
// Данное программное обеспечение, все исключительные права на него, его
// документация и сопроводительные материалы принадлежат ООО «Нанософт разработка».
// Данное программное обеспечение может использоваться при разработке и входить
// в состав разработанных программных продуктов при соблюдении условий
// использования, оговоренных в «Лицензионном договоре присоединения
// на использование программы для ЭВМ «Платформа nanoCAD»».
// 
// Данное программное обеспечение защищено в соответствии с законодательством
// Российской Федерации об интеллектуальной собственности и международными
// правовыми актами.
// 
// Используя данное программное обеспечение,  его документацию и
// сопроводительные материалы вы соглашаетесь с условиями использования,
// указанными выше. 
//

//-----------------------------------------------------------------------------
//- CrossCircleJig.h
#include "..\CrossCircle\AcDbCrossCircle.h"

#pragma once

//-----------------------------------------------------------------------------
class CrossCircleJig : public AcEdJig
{

private:
  //- Member variables
  //- current input level, increment for each input
  int mCurrentInputLevel ;
  //- Dynamic dimension info
  AcDbDimDataPtrArray mDimData ;

public:
  //- Array of input points, each level corresponds to the mCurrentInputLevel
  AcGePoint3dArray mInputPoints ;
  //- Entity being jigged
  AcDbCrossCircle *mpCrCircle;

public:
  CrossCircleJig () ;
  ~CrossCircleJig () ;

  //- Command invoke the jig, call passing a new'd instance of the object to jig
  AcEdJig::DragStatus startJig (AcDbCrossCircle *pEntityToJig) ;

protected:
  //- AcEdJig overrides
  //- input sampler
  virtual DragStatus sampler () ;
  //- jigged entity update
  virtual Adesk::Boolean update () ;
  //- jigged entity pointer return
  virtual AcDbEntity *entity () const ;
  //- dynamic dimension data setup
  virtual AcDbDimDataPtrArray *dimData (const double dimScale) ;
  //- dynamic dimension data update
  virtual Acad::ErrorStatus setDimValue (const AcDbDimData *pDimData, const double dimValue) ;

  //- Standard helper functions
  //- dynamic dimdata update function
  virtual Adesk::Boolean updateDimData () ;

  //- Std input to get a point with no rubber band
  AcEdJig::DragStatus GetStartPoint () ;
  //- Std input to get a point with rubber band from point
  AcEdJig::DragStatus GetNextPoint () ;
} ;
