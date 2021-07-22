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
//----- CrCircleW.h : Declaration of the CCrCircleW
//-----------------------------------------------------------------------------
#pragma once
#include "resource.h"
#include "CrossCircle.h"

// DISPIDs for resources
#define DISPID_CENTER   0x00000001
#define DISPID_RADIUS   0x00000002
#define DISPID_ANGLE    0x00000003

//----- CCrCircleW
class ATL_NO_VTABLE CCrCircleW : 
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<CCrCircleW, &CLSID_CrCircleW>,
  public ISupportErrorInfo,
  public IOPMPropertyExtensionImpl<CCrCircleW>,
  public IOPMPropertyExpander,
  public IAcadEntityDispatchImpl<CCrCircleW, &CLSID_CrCircleW, ICrCircleW, &IID_ICrCircleW, &LIBID_CrossCircleLib>
  //,public IDispatchImpl<Ighfghgfh, &IID_IIghfghgfh, &LIBID_adskArxProject1Lib>
{
public:
  CCrCircleW ()
  {
  }

  DECLARE_REGISTRY_RESOURCEID(IDR_CRCIRCLEW)

  BEGIN_COM_MAP(CCrCircleW)
    COM_INTERFACE_ENTRY(ICrCircleW)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(IOPMPropertyExtension)
    COM_INTERFACE_ENTRY(ICategorizeProperties)
    COM_INTERFACE_ENTRY(IPerPropertyBrowsing)
    COM_INTERFACE_ENTRY(IOPMPropertyExpander)
    COM_INTERFACE_ENTRY(IAcadBaseObject)
//    COM_INTERFACE_ENTRY(IAcadBaseObject2)
    COM_INTERFACE_ENTRY(IAcadObject)
    COM_INTERFACE_ENTRY(IAcadEntity)
    COM_INTERFACE_ENTRY(IRetrieveApplication)
  END_COM_MAP()

  //----- ISupportsErrorInfo
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  HRESULT FinalConstruct ()
  {
    return (S_OK) ;
  }

  void FinalRelease ()
  {
  }

  //IAcadBaseObjectImpl
  virtual HRESULT CreateNewObject (AcDbObjectId &objId, AcDbObjectId &ownerId, TCHAR *keyName);
  //IAcadBaseObject2Impl
  STDMETHOD(ForceDbResident)(VARIANT_BOOL *forceDbResident) ;
  STDMETHOD(CreateObject)(AcDbObjectId ownerId =AcDbObjectId::kNull, TCHAR *keyName =NULL);
  STDMETHOD(AddToDb)(AcDbObjectId &objId, AcDbObjectId ownerId =AcDbObjectId::kNull, TCHAR *keyName =NULL);

  //IOPMPropertyExtension
  BEGIN_OPMPROP_MAP()
    OPMPROP_ENTRY(0, 0x00000001, PROPCAT_Geometry, 0, 0, 0, _T(""), 0, 1, IID_NULL, IID_NULL, "")
    OPMPROP_ENTRY(0, 0x00000002, PROPCAT_Geometry, 0, 0, 0, _T(""), 0, 1, IID_NULL, IID_NULL, "")
    OPMPROP_ENTRY(0, 0x00000003, PROPCAT_Geometry, 0, 0, 0, _T(""), 0, 1, IID_NULL, IID_NULL, "")
  END_OPMPROP_MAP()

  STDMETHOD(GetDisplayName) (DISPID dispId, BSTR *propName);

  //IOPMPropertyExtensionImpl
  virtual HINSTANCE GetResourceInstance () { return (_hdllInstance) ; }

  //IOPMPropertyExpander
  STDMETHOD(GetElementValue)(/*[in]*/DISPID dispID, /*[in]*/DWORD dwCookie, /*[out]*/VARIANT *pVarOut);
  STDMETHOD(SetElementValue)(/*[in]*/DISPID dispID, /*[in]*/DWORD dwCookie, /*[in]*/VARIANT VarIn);
  STDMETHOD(GetElementStrings)(/*[in]*/DISPID dispID, /*[out]*/OPMLPOLESTR __RPC_FAR *pCaStringsOut, /*[out]*/OPMDWORD __RPC_FAR *pCaCookiesOut);
  STDMETHOD(GetElementGrouping)(/*[in]*/DISPID dispID, /*[out]*/short *groupingNumber);
  STDMETHOD(GetGroupCount)(/*[in]*/DISPID dispID, /*[out]*/long *nGroupCnt);

public:
  //ICrCircleW
  STDMETHOD(get_Radius)(/*[out, retval]*/ double *pVal);
  STDMETHOD(put_Radius)(/*[in]*/ double newVal);
  STDMETHOD(get_Center)(/*[out, retval]*/ VARIANT *pVal);
  STDMETHOD(put_Center)(/*[in]*/ VARIANT newVal);
  STDMETHOD(get_AngleD)(/*[out, retval]*/ ACAD_ANGLE *pVal);
  STDMETHOD(put_AngleD)(/*[in]*/ ACAD_ANGLE newVal);

} ;

OBJECT_ENTRY_AUTO(__uuidof(CrCircleW), CCrCircleW)
