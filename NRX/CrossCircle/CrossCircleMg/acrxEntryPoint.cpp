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

#ifdef NCAD
#define CROSSCIRCLE_DBX_NAME L"CrossCircle.nrx"
#else
#define CROSSCIRCLE_DBX_NAME L"CrossCircle.dbx"
#endif

static AcMgObjectFactoryBase **g_CrCircles = NULL;

/////////////////////////////////////////////////////////////////////
// EntryPoint
extern "C" __declspec( dllexport ) AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
  switch (msg)
  {
  case AcRx::kInitAppMsg:
    {
      if (!acrxLoadModule(CROSSCIRCLE_DBX_NAME, 0))
        return AcRx::kRetError;

      acrxDynamicLinker->registerAppMDIAware(pkt);
      // create a new object factory array
      static AcMgObjectFactoryBase* CrCircles[] = 
      {
        new AcMgObjectFactory<HostMgd::Samples::MgCrCrc,AcDbCrossCircle>(), 
        // end the array with a NULL
        NULL
      };

      g_CrCircles = CrCircles;
    }
    break;
  case AcRx::kPreQuitMsg:
    {
      // clean up
      int i=0;
      while (g_CrCircles[i]!=NULL)
        delete g_CrCircles[i++];
    }
    break;
  case AcRx::kUnloadAppMsg:
    acrxUnloadModule(CROSSCIRCLE_DBX_NAME);
    break;
  }
  return AcRx::kRetOK;
}
