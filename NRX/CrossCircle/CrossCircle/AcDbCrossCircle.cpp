//-----------------------------------------------------------------------------
//----- AcDbCrossCircle.cpp : Implementation of AcDbCrossCircle
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AcDbCrossCircle.h"
#include "CrCircleW.h"


Adesk::UInt32 AcDbCrossCircle::kCurrentVersionNumber =1 ;


ACRX_DXF_DEFINE_MEMBERS (
                         AcDbCrossCircle, AcDbEntity,
                         AcDb::kDHL_CURRENT, 
                         AcDb::kMReleaseCurrent, 
                         AcDbProxyEntity::kNoOperation, 
						 ACDBCROSSCIRCLE,
                         CROSSCIRCLEAPP
                         |Product Desc:     A description for your object
                         |Company:          Your company name
                         |WEB Address:      Your company WEB site address
                         )


AcDbCrossCircle::AcDbCrossCircle () : AcDbEntity ()
{
	setCenter( AcGePoint3d( 0, 0, 0 ));

	setVecRad( AcGeVector3d( 1, 0, 0) );

	setNormal( AcGeVector3d( 0, 0, 1 ));
}

AcDbCrossCircle::~AcDbCrossCircle ()
{
}


Acad::ErrorStatus
AcDbCrossCircle::subGetClassID(CLSID* pClsid) const
{
	*pClsid = CLSID_CrCircleW;

	return Acad::eOk;
}

//- Dwg Filing protocol
Acad::ErrorStatus AcDbCrossCircle::dwgOutFields(AcDbDwgFiler *pFiler) const
{
	assertReadEnabled ();

	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;

	if ( es != Acad::eOk )
		return (es) ;

	if ( (es =pFiler->writeUInt32 (AcDbCrossCircle::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;

	pFiler->writePoint3d(m_center);
	pFiler->writeVector3d(m_vecRad);
	pFiler->writeVector3d(m_normal);

	return (pFiler->filerStatus());
}

Acad::ErrorStatus AcDbCrossCircle::dwgInFields(AcDbDwgFiler *pFiler)
{
	assertWriteEnabled () ;

	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;

	if ( es != Acad::eOk )
		return (es) ;

	Adesk::UInt32 version = 0 ;

	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es);

	if ( version > AcDbCrossCircle::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;

	pFiler->readPoint3d(&m_center);
	pFiler->readVector3d(&m_vecRad);
	pFiler->readVector3d(&m_normal);

	return (pFiler->filerStatus ()) ;
}

//- Dxf Filing protocol
Acad::ErrorStatus AcDbCrossCircle::dxfOutFields (AcDbDxfFiler *pFiler) const
{
	assertReadEnabled ();

	Acad::ErrorStatus es =AcDbEntity::dxfOutFields (pFiler) ;

	if ( es != Acad::eOk )
		return (es) ;

	es = pFiler->writeItem (AcDb::kDxfSubclass, _RXST("AcDbCrossCircle")) ;

	if ( es != Acad::eOk )
		return (es) ;

	if ( (es =pFiler->writeUInt32 (AcDb::kDxfInt32, AcDbCrossCircle::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;

	//----- Output params
	es = pFiler->writePoint3d(AcDb::kDxfXCoord, m_center);
	if (es != Acad::eOk) 
	  return es;

	es = pFiler->writeVector3d(AcDb::kDxfNormalX, m_normal, 16);
	if (es != Acad::eOk) 
	  return es;

	es = pFiler->writeVector3d(AcDb::kDxfNormalX+1, m_vecRad,16);
	if (es != Acad::eOk) 
	  return es;

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus AcDbCrossCircle::dxfInFields (AcDbDxfFiler *pFiler)
{
	assertWriteEnabled () ;

	Acad::ErrorStatus es =AcDbEntity::dxfInFields (pFiler) ;

	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("AcDbCrossCircle")) )
		return (pFiler->filerStatus ()) ;

	struct resbuf rb ;
	pFiler->readItem (&rb) ;
	if ( rb.restype != AcDb::kDxfInt32 )
	{
		pFiler->pushBackItem () ;
		pFiler->setError (Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32) ;
		return (pFiler->filerStatus ()) ;
	}

	Adesk::UInt32 version =(Adesk::UInt32)rb.resval.rlong ;

	if ( version > AcDbCrossCircle::kCurrentVersionNumber )
	return (Acad::eMakeMeProxy) ;

	while ( es == Acad::eOk && (es =pFiler->readResBuf (&rb)) == Acad::eOk )
	{
		switch ( rb.restype )
		{
			case AcDb::kDxfXCoord:
			  m_center = asPnt3d(rb.resval.rpoint) ;
			  break;
			case AcDb::kDxfNormalX:
			  m_normal = asVec3d(rb.resval.rpoint);
			  break;
			case AcDb::kDxfNormalX + 1:
			  m_vecRad = asVec3d(rb.resval.rpoint);
			  break;

			default:
			  pFiler->pushBackItem () ;
			  es = Acad::eEndOfFile ;
			  break ;
		}
	}
  
	if ( es != Acad::eEndOfFile )
		return (Acad::eInvalidResBuf) ;

	return (pFiler->filerStatus()) ;
}


Adesk::Boolean AcDbCrossCircle::subWorldDraw (AcGiWorldDraw *mode)
{
	assertReadEnabled() ;
	// Рисуем круг
	mode->subEntityTraits().setSelectionMarker(1);
	mode->geometry().circle(m_center, m_vecRad.length(), m_normal);

	// Рисуем крест
	AcGePoint3d ptArray[2];

	ptArray[0] = m_center + m_vecRad;
	ptArray[1] = m_center - m_vecRad;
	mode->subEntityTraits().setSelectionMarker(2);
	mode->geometry().polyline(2, ptArray);

	ptArray[1] = m_center + 1.1 * m_vecRad;
	mode->geometry().polyline(2, ptArray);

	ptArray[0] = m_center + m_vecRad.crossProduct(m_normal);
	ptArray[1] = m_center - m_vecRad.crossProduct(m_normal);

	mode->subEntityTraits().setSelectionMarker(3);
	mode->geometry().polyline(2, ptArray);

	ptArray[1] = m_center + 1.2 * m_vecRad.crossProduct(m_normal);
	mode->geometry().polyline(2, ptArray);

	return (Adesk::kTrue) ;
}


Adesk::UInt32 AcDbCrossCircle::subSetAttributes (AcGiDrawableTraits *traits)
{
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

//- Osnap points protocol
Acad::ErrorStatus AcDbCrossCircle::subGetOsnapPoints (
  AcDb::OsnapMode osnapMode,
  Adesk::GsMarker gsSelectionMark,
  const AcGePoint3d &pickPoint,
  const AcGePoint3d &lastPoint,
  const AcGeMatrix3d &viewXform,
  AcGePoint3dArray &snapPoints,
  AcDbIntArray &geomIds) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
}

Acad::ErrorStatus AcDbCrossCircle::subGetOsnapPoints (
  AcDb::OsnapMode osnapMode,
  Adesk::GsMarker gsSelectionMark,
  const AcGePoint3d &pickPoint,
  const AcGePoint3d &lastPoint,
  const AcGeMatrix3d &viewXform,
  AcGePoint3dArray &snapPoints,
  AcDbIntArray &geomIds,
  const AcGeMatrix3d &insertionMat) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
}

//- Grip points protocol
Acad::ErrorStatus AcDbCrossCircle::subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds) const
{
	assertReadEnabled () ;

	AcGePoint3dArray aCrCircle;
	aCrCircle.setLogicalLength(9);

	AcGeVector3d vec45 = m_vecRad;
	vec45.rotateBy(atan(1.0), m_normal);

	aCrCircle[0] = m_center + m_vecRad;
	aCrCircle[1] = m_center - m_vecRad;
	aCrCircle[2] = m_center + m_vecRad.crossProduct(m_normal);
	aCrCircle[3] = m_center - m_vecRad.crossProduct(m_normal);
	aCrCircle[4] = m_center + vec45;
	aCrCircle[5] = m_center - vec45;
	aCrCircle[6] = m_center + vec45.crossProduct(m_normal);
	aCrCircle[7] = m_center - vec45.crossProduct(m_normal);
	aCrCircle[8] = m_center;

	gripPoints.append(aCrCircle);

	return Acad::eOk;
}

Acad::ErrorStatus AcDbCrossCircle::subMoveGripPointsAt(const AcDbIntArray &indices, const AcGeVector3d &offset)
{
	if (indices.length()== 0 || offset.isZeroLength())
		return Acad::eOk;

	assertWriteEnabled () ;

	AcGeVector3d vec45 = m_vecRad;
	vec45.rotateBy(atan(1.0), m_normal);

	for (int i = 0; i < indices.length(); i++) 
	{
		switch (indices[i]) 
		{
			case 0: // угол поворота
				this->setVecRad(m_vecRad.length() * (m_vecRad + offset).normalize());
				break;
			case 1:
				this->setVecRad(m_vecRad.length() * (m_vecRad - offset).normalize());
				break;
			case 2:
				this->setVecRad(m_vecRad.length() * (m_vecRad - offset.crossProduct(m_normal)).normalize());
				break;
			case 3:
				this->setVecRad(m_vecRad.length() * (m_vecRad + offset.crossProduct(m_normal)).normalize());
				break;

			case 4: // радиус
				this->setVecRad(m_vecRad.normalize() * (vec45 + offset).length());
				break;
			case 5:
				this->setVecRad(m_vecRad.normalize() * (-vec45 + offset).length());
				break;
			case 6:
				this->setVecRad(m_vecRad.normalize() * (vec45.crossProduct(m_normal) + offset).length());
				break;
			case 7:
				this->setVecRad(m_vecRad.normalize() * (-vec45.crossProduct(m_normal) + offset).length());
				break;

			case 8: // центр
				this->setCenter(m_center + offset);
				break;
		}
	}

  return Acad::eOk;
}

Acad::ErrorStatus AcDbCrossCircle::subGetGripPoints (AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, const AcGeVector3d &curViewDir, const int bitflags) const
{
	assertReadEnabled () ;

	return (AcDbEntity::subGetGripPoints (grips, curViewUnitSize, gripSize, curViewDir, bitflags)) ;
}

Acad::ErrorStatus AcDbCrossCircle::subMoveGripPointsAt (
	  const AcDbVoidPtrArray &gripAppData, 
	  const AcGeVector3d &offset,
	  const int bitflags)
{
	assertWriteEnabled () ;

	return (AcDbEntity::subMoveGripPointsAt (gripAppData, offset, bitflags)) ;
}

Acad::ErrorStatus AcDbCrossCircle::subTransformBy(const AcGeMatrix3d& xform)
{
	assertWriteEnabled();

	m_center.transformBy(xform);
	m_vecRad.transformBy(xform);

	m_normal.transformBy(xform);
	m_normal.normalize();

	return Acad::eOk;
}

Acad::ErrorStatus  AcDbCrossCircle::subExplode(AcDbVoidPtrArray& entitySet) const
{
	assertReadEnabled();

	Acad::ErrorStatus es = Acad::eOk;

	// Разбиваем окружность на составляющие.
	// Круг
	AcDbCircle* circle;

	circle = new AcDbCircle(center(), normal(), VecRad().length());
	entitySet.append(circle);

	AcGePoint3d pt1, pt2;

	// Первый отрезок
	AcDbLine* line;

	pt1 = center() + VecRad();
	pt2 = center() - VecRad();

	line = new AcDbLine();
	line->setStartPoint(pt1);
	line->setEndPoint(pt2);
	line->setNormal(normal());
	entitySet.append(line);

	// Второй отрезок
	pt1 = center() + VecRad().crossProduct(normal());
	pt2 = center() - VecRad().crossProduct(normal());
	line = new AcDbLine();
	line->setStartPoint(pt1);
	line->setEndPoint(pt2);
	line->setNormal(normal());
	entitySet.append(line);

	return es;
}


AcGePoint3d AcDbCrossCircle::center() const
{
	assertReadEnabled();
	return m_center;
}

Acad::ErrorStatus AcDbCrossCircle::setCenter(AcGePoint3d center)
{
	assertWriteEnabled();
	m_center = center;
	return Acad::eOk;
}

AcGeVector3d AcDbCrossCircle::VecRad() const
{
	assertReadEnabled();
	return m_vecRad;
}

Acad::ErrorStatus AcDbCrossCircle::setVecRad(AcGeVector3d VecRad)
{
	assertWriteEnabled();
	m_vecRad = VecRad;
	return Acad::eOk;
}

AcGeVector3d AcDbCrossCircle::normal() const
{
	assertReadEnabled();
	return m_normal;
}

Acad::ErrorStatus AcDbCrossCircle::setNormal(AcGeVector3d normal)
{
	assertWriteEnabled();
	m_normal = normal;
	return Acad::eOk;
}

double AcDbCrossCircle::radius() const
{
	assertReadEnabled();
	return m_vecRad.length();
}

Acad::ErrorStatus AcDbCrossCircle::setRadius(double radius)
{
	assertWriteEnabled();

	m_vecRad = radius * m_vecRad.normalize();

	return Acad::eOk;
}

double AcDbCrossCircle::angle() const
{
	assertReadEnabled();

	AcGeVector3d x;
	double ang;
	x = acdbHostApplicationServices()->workingDatabase()->ucsxdir();
	ang = m_vecRad.angleTo(x, -m_normal);

	return (fabs(ang - 8*atan(1.0)) < 1e-6) ? 0 : ang;
}

Acad::ErrorStatus AcDbCrossCircle::setAngle(double angle)
{
	assertWriteEnabled();

	AcGeVector3d x = acdbHostApplicationServices()->workingDatabase()->ucsxdir();

	m_vecRad = m_vecRad.rotateBy(angle - m_vecRad.angleTo(x, -m_normal), m_normal) ;

	return Acad::eOk;
}
