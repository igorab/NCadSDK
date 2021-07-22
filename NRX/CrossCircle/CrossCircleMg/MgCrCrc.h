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
//- MgCrCrc.h
#pragma once

//-----------------------------------------------------------------------------
#if NCAD
using namespace HostMgd::ApplicationServices;
using namespace HostMgd::EditorInput;
using namespace Teigha::Geometry;
using namespace Teigha::Runtime;

namespace NativePlatform = Teigha;
namespace WrapperPlatform = NrxGate;

#else
using namespace System ;
using namespace Autodesk::AutoCAD::Geometry ;
using namespace Autodesk::AutoCAD::DatabaseServices ;

namespace NativePlatform = Autodesk::AutoCAD;
namespace WrapperPlatform = Autodesk::AutoCAD;
#endif

namespace HostMgd {
  namespace Samples {

    [NativePlatform::Runtime::Wrapper("AcDbCrossCircle")]
    public ref class MgCrCrc : public WrapperPlatform::DatabaseServices::Entity
    {

    public:
      //- Constructor
      MgCrCrc () ;

    internal:
      MgCrCrc (System::IntPtr unmanagedPointer, bool bAutoDelete) ;

      //- Returns the unmanaged ARX Object
      inline AcDbCrossCircle *GetImpObj ()
      {
        return static_cast<AcDbCrossCircle *>(UnmanagedObject.ToPointer ()) ;
      }

    public:
      //- To define properties which get/set values of your object
      //- the format you must use is
      property Point3d mycenter
      {
        void set(Point3d point);
        Point3d get();
      }

      property Vector3d mydirection
      {
        void set(Vector3d vec);
        Vector3d get();
      }

      property Vector3d mynormal
      {
        void set(Vector3d vec);
        Vector3d get();
      }
    };
  }
}