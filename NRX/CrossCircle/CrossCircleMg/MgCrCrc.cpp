//
// �������� (�) 2019, ��� ��������� ����������. ��� ����� ��������.
// 
// ������ ����������� �����������, ��� �������������� ����� �� ����, ���
// ������������ � ���������������� ��������� ����������� ��� ��������� ����������.
// ������ ����������� ����������� ����� �������������� ��� ���������� � �������
// � ������ ������������� ����������� ��������� ��� ���������� �������
// �������������, ����������� � ������������� �������� �������������
// �� ������������� ��������� ��� ��� ���������� nanoCAD��.
// 
// ������ ����������� ����������� �������� � ������������ � �����������������
// ���������� ��������� �� ���������������� ������������� � ��������������
// ��������� ������.
// 
// ��������� ������ ����������� �����������,  ��� ������������ �
// ���������������� ��������� �� ������������ � ��������� �������������,
// ���������� ����. 
//

//-----------------------------------------------------------------------------
#include "StdAfx.h"

//-----------------------------------------------------------------------------
//- Template class that wraps GCHandle from mscorlib.dll
#include <gcroot.h>

#ifdef NCAD
//-----------------------------------------------------------------------------
HostMgd::Samples::MgCrCrc::MgCrCrc () 
: WrapperPlatform::DatabaseServices::Entity (WrapperPlatform::UnmanagedPointerType::eWrapper, (System::IntPtr)new AcDbCrossCircle(), true)
{
}

//-----------------------------------------------------------------------------
HostMgd::Samples::MgCrCrc::MgCrCrc (System::IntPtr unmanagedPointer, bool bAutoDelete)
: WrapperPlatform::DatabaseServices::Entity (WrapperPlatform::UnmanagedPointerType::eNative, unmanagedPointer, bAutoDelete)
{
}

#else
//-----------------------------------------------------------------------------
HostMgd::Samples::MgCrCrc::MgCrCrc () 
: WrapperPlatform::DatabaseServices::Entity ((System::IntPtr)new AcDbCrossCircle (), true)
{
}

//-----------------------------------------------------------------------------
HostMgd::Samples::MgCrCrc::MgCrCrc (System::IntPtr unmanagedPointer, bool bAutoDelete)
: WrapperPlatform::DatabaseServices::Entity (unmanagedPointer, bAutoDelete)
{
}

#endif

//////////////////////////////////////////////////////////////////////////
// set the centre point
void HostMgd::Samples::MgCrCrc::mycenter::set(Point3d point)
{
  NativePlatform::Runtime::Interop::Check(GetImpObj()->setCenter(*reinterpret_cast<AcGePoint3d*>(&(point))));
}
//////////////////////////////////////////////////////////////////////////
// get the center point
Point3d HostMgd::Samples::MgCrCrc::mycenter::get()
{
  Point3d ret;
  *reinterpret_cast<AcGePoint3d*>(&(ret)) = GetImpObj()->center();
  return ret;
}

  //////////////////////////////////////////////////////////////////////////
  // set the direction radius and radius
  void HostMgd::Samples::MgCrCrc::mydirection::set(Vector3d vec)
  {
    NativePlatform::Runtime::Interop::Check(GetImpObj()->setVecRad(*reinterpret_cast<AcGeVector3d*>(&(vec))));
  }
  //////////////////////////////////////////////////////////////////////////
  // get the direction radius and radius
  Vector3d HostMgd::Samples::MgCrCrc::mydirection::get()
  {
    Vector3d ret;
    *reinterpret_cast<AcGeVector3d*>(&(ret)) = GetImpObj()->VecRad();
    return ret;
  }

  //////////////////////////////////////////////////////////////////////////
  // set the normal
  void HostMgd::Samples::MgCrCrc::mynormal::set(Vector3d vec)
  {
    NativePlatform::Runtime::Interop::Check(GetImpObj()->setNormal(*reinterpret_cast<AcGeVector3d*>(&(vec))));
  }
  //////////////////////////////////////////////////////////////////////////
  // get the direction radius and radius
  Vector3d HostMgd::Samples::MgCrCrc::mynormal::get()
  {
    Vector3d ret;
    *reinterpret_cast<AcGeVector3d*>(&(ret)) = GetImpObj()->normal();
    return ret;
  }
