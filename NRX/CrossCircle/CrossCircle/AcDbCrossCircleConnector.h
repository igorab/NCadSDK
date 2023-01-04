#pragma once
#include <database.h>


#ifdef CROSSCIRCLE_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP
#endif

class DLLIMPEXP AcDbCrossCircleConnector : public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(AcDbCrossCircleConnector);

protected:
	static Adesk::UInt32 kCurrentVersionNumber;

public:

	AcDbCrossCircleConnector();
	AcDbCrossCircleConnector(const AcGePoint3d& center, const AcGeVector3d& normal, const AcGeVector3d& vecRadius, const double& length);
	virtual ~AcDbCrossCircleConnector() {};

	AcGePoint3d center() const;

	Acad::ErrorStatus setCenter(const AcGePoint3d &center);

	AcGeVector3d vecRadius() const;

	Acad::ErrorStatus setVecRadius(const AcGeVector3d &vecRadius);

	AcGeVector3d normal() const;

	Acad::ErrorStatus setNormal(const AcGeVector3d &normal);

	double length() const;

	Acad::ErrorStatus setLength(const double &length);

	double radius() const;

	Acad::ErrorStatus setRadius(const double &radius);

	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* pFiler) const override;

	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler* pFiler) override;

	virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints, AcDbIntArray& osnapModes, AcDbIntArray& geomIds) const override;

	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, const AcGeVector3d& offset) override;

protected:
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode) override;

	virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform) override;

	virtual Acad::ErrorStatus subExplode(AcDbVoidPtrArray& entitySet) const override;

private:

	AcGeMatrix3d matrix() const;

	AcGePoint3d m_center;  // центр окружности
	AcGeVector3d m_vecRadius; // вектор радиуса
	AcGeVector3d m_normal; // нормаль
	double m_length; // длина лапок
};


#ifdef CROSSCIRCLE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(AcDbCrossCircleConnector);
#endif


