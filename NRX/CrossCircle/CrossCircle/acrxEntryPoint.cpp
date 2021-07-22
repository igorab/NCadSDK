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
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "AcDbCrossCircle.h"

//-----------------------------------------------------------------------------
//----- EntryPoint
class CCrossCircleApp : public AcRxDbxApp
{

public:
  CCrossCircleApp () : AcRxDbxApp () {}

  virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt)
  {

    // You *must* call On_kInitAppMsg here
    AcRx::AppRetCode retCode =AcRxDbxApp::On_kInitAppMsg (pkt) ;

    return (retCode) ;
  }

  virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt)
  {

    // You *must* call On_kUnloadAppMsg here
    AcRx::AppRetCode retCode =AcRxDbxApp::On_kUnloadAppMsg (pkt) ;

    return (retCode) ;
  }

  virtual void RegisterServerComponents ()
  {
    // ----- Self-register COM server upon loading.
    if ( FAILED(::DllRegisterServer ()) )
      acutPrintf (_RXST("CrossCircle: Cannot self-register COM server, HKLM is not writable.\n")) ;
  }

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CCrossCircleApp)
