#include "StdAfx.h"
#include "CrossCircleJig.h"

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


// Jigged entity pointer return
AcDbEntity *CrossCircleJig::entity () const
{
	return ((AcDbEntity *)mpCrCircle) ;
}

//-----------------------------------------------------------------------------
//- Dynamic dimension data setup
AcDbDimDataPtrArray *CrossCircleJig::dimData (const double dimScale)
{
	return (NULL);
}

// Dynamic dimension data update
Acad::ErrorStatus CrossCircleJig::setDimValue (const AcDbDimData *pDimData, const double dimValue)
{
	Acad::ErrorStatus es = Acad::eOk ;

	return (es) ;
}

// Various helper functions
// Dynamic dimdata update function
Adesk::Boolean CrossCircleJig::updateDimData()
{
	if (mDimData.length () <= 0)
		return (true) ;

	return (true) ;
}

// Std input to get a point with no rubber band
AcEdJig::DragStatus CrossCircleJig::GetStartPoint ()
{
	AcGePoint3d newPnt ;

	AcEdJig::DragStatus status = acquirePoint (newPnt);

	if ( status == AcEdJig::kNormal )
	{
		if (newPnt.isEqualTo (mInputPoints[mCurrentInputLevel]))
			return (AcEdJig::kNoChange) ;

		// Otherwise update the point
		mInputPoints [mCurrentInputLevel] = newPnt;
	}

	return (status) ;
}


//- Std input to get a point with rubber band from point
AcEdJig::DragStatus CrossCircleJig::GetNextPoint ()
{
	AcGePoint3d newPnt ;
	// Get the point 
	AcEdJig::DragStatus status =acquirePoint (newPnt, mInputPoints [mCurrentInputLevel-1]) ;

	// If valid input
	if ( status == AcEdJig::kNormal )
	{
		// If there is no difference
		if ( newPnt.isEqualTo (mInputPoints [mCurrentInputLevel]) )
            return (AcEdJig::kNoChange);

		//- Otherwise update the point
		mInputPoints [mCurrentInputLevel] = newPnt;
	}
	return (status) ;
}
