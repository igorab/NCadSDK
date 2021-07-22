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
#include "StdAfx.h"
#include "CrossCircleJig.h"

//-----------------------------------------------------------------------------
CrossCircleJig::CrossCircleJig () : AcEdJig (),
mCurrentInputLevel(0), mpCrCircle(NULL)
{
}

CrossCircleJig::~CrossCircleJig ()
{
}

//-----------------------------------------------------------------------------
AcEdJig::DragStatus CrossCircleJig::startJig (AcDbCrossCircle *pEntity)
{
  //- Store the new entity pointer
  mpCrCircle =pEntity ;

  //- Setup each input prompt
  ACHAR* inputPrompts [2] = 
  {
    _T("\nЦентр круга: "),
    _T("\nРадиус круга: ")
  } ;

  bool appendOk =true ;
  AcEdJig::DragStatus status =AcEdJig::kNull ;
  //- Loop the number of inputs
  for ( mCurrentInputLevel =0 ; mCurrentInputLevel < 2 ; mCurrentInputLevel++ ) 
  {
    //- Add a new input point to the list of input points
    mInputPoints.append (AcGePoint3d ()) ;

    //- Set the input prompt
    setDispPrompt (inputPrompts [mCurrentInputLevel]) ;

    bool quit =false ;
    //- Lets now do the input
    status =drag () ;
    if ( status != kNormal ) 
    {
      //- If it's a keyword
      switch ( status ) 
      {
      case kCancel: 
      case kNull:
        quit =true ;
        appendOk =false ;
        break ;

      case kKW1:
      case kKW2:
      case kKW3:
      case kKW4:
      case kKW5:
      case kKW6:
      case kKW7:
      case kKW8:
      case kKW9:
        //- Do something

        break ;
      }
    } else
    {
      appendOk =true ;
    }

    //- If to finish
    if ( quit )
      break ;
  }

  //- If the input went well
  if ( appendOk )
    //- Append to the database
    append () ;
  else
    //- Clean up
    delete mpCrCircle  ;

  return (status) ;
}

//-----------------------------------------------------------------------------
//- Input sampler
AcEdJig::DragStatus CrossCircleJig::sampler ()
{
  //- Setup the user input controls for each input
  AcEdJig::UserInputControls userInputControls [2] =
  {
    (UserInputControls)	(AcEdJig::kAccept3dCoordinates 
    | AcEdJig::kNoNegativeResponseAccepted),
    (UserInputControls) (AcEdJig::kAccept3dCoordinates
    | AcEdJig::kGovernedByOrthoMode
    | AcEdJig::kAcceptOtherInputString  
    | AcEdJig::kNoNegativeResponseAccepted
    | AcEdJig::kNullResponseAccepted
    | AcEdJig::kNoZeroResponseAccepted)
  } ;
  setUserInputControls (userInputControls [mCurrentInputLevel]) ;

  AcEdJig::DragStatus status =AcEdJig::kCancel ;
  //- Check the current input number to see which input to do
  switch ( mCurrentInputLevel+1 )
  {
  case 1:
    status =GetStartPoint () ;
    break ;
  case 2:
    status =GetNextPoint () ;
    break ;
  default:
    break ;
  }
  return (status) ;
}

//-----------------------------------------------------------------------------
//- Jigged entity update
Adesk::Boolean CrossCircleJig::update ()
{
  //- Check the current input number to see which update to do
  switch ( mCurrentInputLevel+1 )
  {
  case 1:
    {
      // Acquire current UCS transformation matrix.
      //
      AcGeMatrix3d UcsToWcsMat;
      acdbUcsMatrix(UcsToWcsMat);

      // Get data from user coordinate system.
      //
      AcGePoint3d orgPt;
      AcGeVector3d xAxis, yAxis, zAxis;
      UcsToWcsMat.getCoordSystem(orgPt, xAxis, yAxis, zAxis);

      mpCrCircle->setDatabaseDefaults();
      mpCrCircle->setCenter(mInputPoints [mCurrentInputLevel]) ;
      mpCrCircle->setNormal(zAxis);
    }
    break ;
  case 2:
    {
      // Plane of construction
      AcGeVector3d acqVector = mInputPoints [mCurrentInputLevel] - mInputPoints [mCurrentInputLevel-1];

      mpCrCircle->setVecRad(acqVector.length() * acqVector.orthoProject(mpCrCircle->normal()).normalize());
    }
    break ;
  default:
    break ;
  }

  return (updateDimData ()) ;
}

//-----------------------------------------------------------------------------
//- Jigged entity pointer return
AcDbEntity *CrossCircleJig::entity () const
{
  return ((AcDbEntity *)mpCrCircle) ;
}

//-----------------------------------------------------------------------------
//- Dynamic dimension data setup
AcDbDimDataPtrArray *CrossCircleJig::dimData (const double dimScale)
{

  /* SAMPLE CODE:
  AcDbAlignedDimension *dim =new AcDbAlignedDimension () ;
  dim->setDatabaseDefaults () ;
  dim->setNormal (AcGeVector3d::kZAxis) ;
  dim->setElevation (0.0) ;
  dim->setHorizontalRotation (0.0) ;
  dim->setXLine1Point (m_originPoint) ;
  dim->setXLine2Point (m_lastPoint) ;
  //- Get the dimPoint, first the midpoint
  AcGePoint3d dimPoint =m_originPoint + ((m_lastPoint - m_originPoint) / 2.0) ;
  //- Then the offset
  dim->setDimLinePoint (dimPoint) ;
  dim->setDimtad (1) ;

  AcDbDimData *dimData = new AcDbDimData (dim) ;
  //AppData *appData =new AppData (1, dimScale) ;
  //dimData.setAppData (appData) ;
  dimData->setDimFocal (true) ;
  dimData->setDimHideIfValueIsZero (true) ;

  //- Check to see if it is required
  if ( getDynDimensionRequired (m_inputNumber) )
  dimData->setDimInvisible (false) ;
  else
  dimData->setDimInvisible (true) ;

  //- Make sure it is editable TODO: 
  dimData->setDimEditable (true) ;
  mDimData.append (dimData) ;

  return (&mDimData) ;
  */
  return (NULL) ;
}

//-----------------------------------------------------------------------------
//- Dynamic dimension data update
Acad::ErrorStatus CrossCircleJig::setDimValue (const AcDbDimData *pDimData, const double dimValue)
{
  Acad::ErrorStatus es =Acad::eOk ;

  /* SAMPLE CODE:
  //- Convert the const pointer to non const
  AcDbDimData *dimDataNC =const_cast<AcDbDimData *>(pDimData) ;
  int inputNumber =-1 ;
  //- Find the dim data being passed so we can determine the input number
  if ( mDimData.find (dimDataNC, inputNumber) )
  {
  //- Now get the dimension
  AcDbDimension *pDim =(AcDbDimension *)dimDataNC->dimension () ;
  //- Check it's the type of dimension we want
  AcDbAlignedDimension *pAlnDim =AcDbAlignedDimension::cast (pDim) ;
  //- If ok
  if ( pAlnDim )
  {
  //- Extract the dimensions as they are now
  AcGePoint3d dimStart =pAlnDim->xLine1Point () ;
  AcGePoint3d dimEnd =pAlnDim->xLine2Point () ;
  //- Lets get the new point entered by the user 
  AcGePoint3d dimEndNew =dimStart + (dimEnd - dimStart).normalize () * dimValue ;
  //- Finally set the end dim point
  pAlnDim->setXLine2Point (dimEndNew) ;
  //- Now update the jig data to reflect the dynamic dimension input
  mInputPoints [mCurrentInputLevel] =dimEndNew ;
  }
  }*/
  return (es) ;
}

//-----------------------------------------------------------------------------
//- Various helper functions
//- Dynamic dimdata update function
Adesk::Boolean CrossCircleJig::updateDimData ()
{
  //- Check the dim data store for validity
  if ( mDimData.length () <= 0 )
    return (true) ;

  /* SAMPLE CODE :
  //- Extract the individual dimData
  AcDbDimData *dimData =mDimData [m_inputNumber] ;
  //- Now get the dimension
  AcDbDimension *pDim =(AcDbDimension *)dimData->dimension () ;
  //- Check it's the type of dimension we want
  AcDbAlignedDimension *pAlnDim =AcDbAlignedDimension::cast (pDim) ;
  //- If ok
  if ( pAlnDim )
  {
  //- Check to see if it is required
  if ( getDynDimensionRequired (m_inputNumber) )
  dimData->setDimInvisible (false) ;
  else
  dimData->setDimInvisible (true) ;
  pAlnDim->setXLine1Point (m_originPoint) ;
  pAlnDim->setXLine2Point (m_lastPoint) ;
  //- Get the dimPoint, first the midpoint
  AcGePoint3d dimPoint =m_originPoint + ((m_lastPoint - m_originPoint) / 2.0) ;
  //- Then the offset
  pAlnDim->setDimLinePoint (dimPoint) ;
  } */
  return (true) ;
}

//-----------------------------------------------------------------------------
//- Std input to get a point with no rubber band
AcEdJig::DragStatus CrossCircleJig::GetStartPoint ()
{
  AcGePoint3d newPnt ;
  //- Get the point 
  AcEdJig::DragStatus status =acquirePoint (newPnt) ;
  //- If valid input
  if ( status == AcEdJig::kNormal )
  {
    //- If there is no difference
    if ( newPnt.isEqualTo (mInputPoints [mCurrentInputLevel]) )
      return (AcEdJig::kNoChange) ;
    //- Otherwise update the point
    mInputPoints [mCurrentInputLevel] =newPnt ;
  }
  return (status) ;
}

//-----------------------------------------------------------------------------
//- Std input to get a point with rubber band from point
AcEdJig::DragStatus CrossCircleJig::GetNextPoint ()
{
  AcGePoint3d newPnt ;
  //- Get the point 
  AcEdJig::DragStatus status =acquirePoint (newPnt, mInputPoints [mCurrentInputLevel-1]) ;
  //- If valid input
  if ( status == AcEdJig::kNormal )
  {
    //- If there is no difference
    if ( newPnt.isEqualTo (mInputPoints [mCurrentInputLevel]) )
      return (AcEdJig::kNoChange) ;
    //- Otherwise update the point
    mInputPoints [mCurrentInputLevel] =newPnt ;
  }
  return (status) ;
}
