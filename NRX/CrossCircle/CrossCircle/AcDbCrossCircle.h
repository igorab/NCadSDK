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
//----- AcDbCrossCircle.h : Declaration of the AcDbCrossCircle
//-----------------------------------------------------------------------------
#pragma once

#ifdef CROSSCIRCLE_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
class DLLIMPEXP AcDbCrossCircle : public AcDbEntity
{

public:
  ACRX_DECLARE_MEMBERS(AcDbCrossCircle) ;

protected:
  static Adesk::UInt32 kCurrentVersionNumber ;

public:
  AcDbCrossCircle () ;
  virtual ~AcDbCrossCircle () ;

  /////////////////////////////////////////////////////////
  // Methods specific to AcDbCrossCircle
  /////////////////////////////////////////////////////////
  // Data Members access functions

  AcGePoint3d center() const;
  Acad::ErrorStatus setCenter(AcGePoint3d pIns);
  AcGeVector3d VecRad() const;
  Acad::ErrorStatus setVecRad(AcGeVector3d VecRad);
  AcGeVector3d	  normal() const;
  Acad::ErrorStatus setNormal(AcGeVector3d normal);
  double radius() const;
  Acad::ErrorStatus setRadius(double radius);
  double angle() const;
  Acad::ErrorStatus setAngle(double angle);

  //----- AcDbObject protocols

  virtual Acad::ErrorStatus subGetClassID(CLSID* pClsid) const override;

  //- Dwg Filing protocol
  virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const override;
  virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) override;

  //- Dxf Filing protocol
  virtual Acad::ErrorStatus dxfOutFields (AcDbDxfFiler *pFiler) const override;
  virtual Acad::ErrorStatus dxfInFields (AcDbDxfFiler *pFiler) override;

  //----- AcDbEntity protocols
  //- Graphics protocol
protected:
  virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) override;
  virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) override;

  virtual Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform) override;
  virtual Acad::ErrorStatus subExplode(AcDbVoidPtrArray& entitySet) const override;

  //- Osnap points protocol
public:
  virtual Acad::ErrorStatus subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    Adesk::GsMarker gsSelectionMark,
    const AcGePoint3d &pickPoint,
    const AcGePoint3d &lastPoint,
    const AcGeMatrix3d &viewXform,
    AcGePoint3dArray &snapPoints,
    AcDbIntArray &geomIds) const override;
  virtual Acad::ErrorStatus subGetOsnapPoints (
    AcDb::OsnapMode osnapMode,
    Adesk::GsMarker gsSelectionMark,
    const AcGePoint3d &pickPoint,
    const AcGePoint3d &lastPoint,
    const AcGeMatrix3d &viewXform,
    AcGePoint3dArray &snapPoints,
    AcDbIntArray &geomIds,
    const AcGeMatrix3d &insertionMat) const override;

  //- Grip points protocol
  virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const override;
  virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) override;
  virtual Acad::ErrorStatus subGetGripPoints (
    AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
    const AcGeVector3d &curViewDir, const int bitflags) const override;
  virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset, const int bitflags) override;

private:
  // Data Members
  AcGePoint3d m_center;  // центр окружности
  AcGeVector3d m_vecRad; // вектор радиуса
  AcGeVector3d m_normal; // нормаль

};

#ifdef CROSSCIRCLE_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(AcDbCrossCircle);
#endif
