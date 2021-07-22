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