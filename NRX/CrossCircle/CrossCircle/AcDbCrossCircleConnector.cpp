#include "StdAfx.h"
#include "AcDbCrossCircleConnector.h"

Adesk::UInt32 AcDbCrossCircleConnector::kCurrentVersionNumber = 1;

ACRX_DXF_DEFINE_MEMBERS (
    AcDbCrossCircleConnector, AcDbEntity,
    AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
    AcDbProxyEntity::kNoOperation, ACDBCROSSCIRCLECONNECTOR,
	CROSSCIRCLEAPP
	| Product Desc : A description for your product
	| Company : Your company name
	| WEB Address : Your company WEB site address
)

AcDbCrossCircleConnector::AcDbCrossCircleConnector() : 
    AcDbEntity(), m_center(AcGePoint3d::kOrigin), m_normal(AcGeVector3d::kZAxis), m_vecRadius(AcGeVector3d(20, 0, 0)), m_length(10)
{

}

AcDbCrossCircleConnector::AcDbCrossCircleConnector(const AcGePoint3d& center, const AcGeVector3d& normal, const AcGeVector3d& vecRadius, const double& length) :
    AcDbEntity(), m_center(center), m_normal(normal), m_length(length)

{
    setVecRadius(vecRadius.length() * vecRadius.orthoProject(this->normal()).normalize());
}

AcGePoint3d AcDbCrossCircleConnector::center() const
{
    assertReadEnabled();

    return m_center;
}

Acad::ErrorStatus AcDbCrossCircleConnector::setCenter(const AcGePoint3d& center)
{
    assertWriteEnabled();

    m_center = center;
    return Acad::eOk;
}

AcGeVector3d AcDbCrossCircleConnector::vecRadius() const
{
    assertReadEnabled();
    
    return m_vecRadius;
}

Acad::ErrorStatus AcDbCrossCircleConnector::setVecRadius(const AcGeVector3d& vecRadius)
{
    assertWriteEnabled();
    
    m_vecRadius = vecRadius;
    return Acad::eOk;
}

AcGeVector3d AcDbCrossCircleConnector::normal() const
{
    assertReadEnabled();
    
    return m_normal;
}

Acad::ErrorStatus AcDbCrossCircleConnector::setNormal(const AcGeVector3d& normal)
{
    assertWriteEnabled();
    
    m_normal = normal;
    return Acad::eOk;
}


double AcDbCrossCircleConnector::length() const
{
    assertReadEnabled();
    
    return m_length;
}

Acad::ErrorStatus AcDbCrossCircleConnector::setLength(const double& length)
{
    assertWriteEnabled();

    m_length = length;

    recordGraphicsModified();

    return Acad::eOk;
}

double AcDbCrossCircleConnector::radius() const
{
    assertReadEnabled();

    return m_vecRadius.length();
}

Acad::ErrorStatus AcDbCrossCircleConnector::setRadius(const double& radius)
{
    assertWriteEnabled();

    m_vecRadius = radius * m_vecRadius.normalize();
    return Acad::eOk;
}

Acad::ErrorStatus AcDbCrossCircleConnector::dwgOutFields(AcDbDwgFiler* pFiler) const
{
    assertReadEnabled();

    Acad::ErrorStatus es = AcDbEntity::dwgOutFields(pFiler);
    if (es != Acad::eOk)
        return (es);
 
    if ((es = pFiler->writeUInt32(AcDbCrossCircleConnector::kCurrentVersionNumber)) != Acad::eOk)
        return (es);

    pFiler->writePoint3d(m_center);
    pFiler->writeVector3d(m_normal);
	pFiler->writeVector3d(m_vecRadius);
    pFiler->writeDouble(m_length);

    return pFiler->filerStatus();
}

Acad::ErrorStatus AcDbCrossCircleConnector::dwgInFields(AcDbDwgFiler* pFiler)
{
    assertWriteEnabled();

    Acad::ErrorStatus es = AcDbEntity::dwgInFields(pFiler);
    if (es != Acad::eOk)
        return (es);
 
    Adesk::UInt32 version = 0;
    if ((es = pFiler->readUInt32(&version)) != Acad::eOk)
        return (es);

    if (version > AcDbCrossCircleConnector::kCurrentVersionNumber)
        return (Acad::eMakeMeProxy);
 
    pFiler->readPoint3d(&m_center);
    pFiler->readVector3d(&m_normal);
	pFiler->readVector3d(&m_vecRadius);
    pFiler->readDouble(&m_length);

    return pFiler->filerStatus();
}

Acad::ErrorStatus AcDbCrossCircleConnector::subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds) const
{
    assertReadEnabled();

    AcGePoint3dArray pntArray;
    pntArray.setLogicalLength(9);

    pntArray[0] = m_center + m_vecRadius;
    pntArray[1] = m_center - m_vecRadius.crossProduct(m_normal);

    AcGeVector3d vecRotation = m_vecRadius;
    double angle = PI / 3;
    for (unsigned int i = 0; i < 6; ++i)
    {
        AcGeVector3d vec = vecRotation;
        pntArray[i + 2] = m_center + vecRotation + m_length * vec.normalize();
        vecRotation = vecRotation.rotateBy(angle, AcGeVector3d::kZAxis);
    }
    pntArray[8] = center();

    gripPoints.append(pntArray);

    return Acad::eOk;
}

Acad::ErrorStatus AcDbCrossCircleConnector::subMoveGripPointsAt(const AcDbIntArray& indices, const AcGeVector3d& offset)
{
    Acad::ErrorStatus es = Acad::eOk;
	
	if (indices.length() == 0 || offset.isZeroLength())
		return Acad::eOk;

	assertWriteEnabled();
	
    AcGeVector3d newOffset = offset*0.5;

    AcGeVector3d vecOrientation = m_vecRadius.normal();
	double lenOX = vecOrientation.dotProduct(offset);

    for (unsigned int i = 0; i < indices.length(); i++)
    {
        switch (indices[i])
        {
        case 0: // радиус
            if (lenOX > 0)
                this->setRadius(radius() + offset.length());
            else
                this->setRadius(radius() - offset.length());
            break;
        case 1: // поворот
            this->setVecRadius(radius() * (m_vecRadius - offset).normalize());
            break;
        case 2: // лапки
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        {
            AcGeVector3d vecRotation = m_vecRadius;
            double angle = PI / 3;
            vecRotation = vecRotation.rotateBy(angle*(indices[i]-2), AcGeVector3d::kZAxis);
            double x = vecRotation.dotProduct(offset);
            if (x > 0) {
                this->setLength(m_length + offset.length());
            }
            else {
                this->setLength(m_length - offset.length());
            }
        }
            break;
        case 8: // центр
            this->setCenter(m_center + newOffset);
            break;
        }
    }

    return es;
}

Adesk::Boolean AcDbCrossCircleConnector::subWorldDraw(AcGiWorldDraw* mode)
{
    assertReadEnabled();

    AcGeMatrix3d matrix = this->matrix();
    mode->subEntityTraits().setSelectionMarker(1);
    mode->geometry().pushModelTransform(matrix);
    mode->geometry().circle(AcGePoint3d::kOrigin, radius(), m_normal);

    AcGePoint3d pntArray[2];
    AcGeVector3d vecRotation = m_vecRadius;
    vecRotation.transformBy(matrix);
    double angle = PI / 3;
    for (unsigned int i = 0; i < 6; ++i) {
        pntArray[0] = AcGePoint3d::kOrigin + vecRotation;
        AcGeVector3d vec = vecRotation;
        pntArray[1] = pntArray[0] + m_length * vec.normalize();
        mode->geometry().polyline(2, pntArray);
        vecRotation = vecRotation.rotateBy(angle, AcGeVector3d::kZAxis);
    }

    mode->geometry().popModelTransform();

    return Adesk::kTrue;
}

Acad::ErrorStatus AcDbCrossCircleConnector::subTransformBy(const AcGeMatrix3d& xform)
{
    assertWriteEnabled();

    m_center.transformBy(xform);
    m_vecRadius.transformBy(xform);

    m_normal.transformBy(xform);
    m_normal.normalize();

    return Acad::eOk;
}

Acad::ErrorStatus AcDbCrossCircleConnector::subExplode(AcDbVoidPtrArray& entitySet) const
{
    assertReadEnabled();

    AcGeMatrix3d matrix = this->matrix();
    Acad::ErrorStatus es = Acad::eOk;

    AcDbCircle* circle;
    circle = new AcDbCircle(center().transformBy(matrix), m_normal, radius());

    entitySet.append(circle);

    AcGePoint3d pt1, pt2;
    AcDbLine* line = nullptr;

    AcGeVector3d vecRotation = m_vecRadius;
    double angle = PI / 3;
    for (size_t i = 0; i < 6; ++i) {
        pt1 = (m_center + vecRotation).transformBy(matrix);
        AcGeVector3d vec = vecRotation;
        pt2 = pt1 + m_length * vec.normalize();
        line = new AcDbLine();
        line->setStartPoint(pt1);
        line->setEndPoint(pt2);
        line->setNormal(m_normal);
        vecRotation = vecRotation.rotateBy(angle, AcGeVector3d::kZAxis);
        entitySet.append(line);
    }

    return es;
}

AcGeMatrix3d AcDbCrossCircleConnector::matrix() const
{
    AcGeMatrix3d matrix;
    AcGeVector3d vecOrientation = m_vecRadius.normal();
    return matrix.setCoordSystem(m_center, vecOrientation, vecOrientation.crossProduct(m_normal), m_normal);
}
