//CrCircleW.cpp : Implementation of CCrCircleW
#include "StdAfx.h"
#include "CrCircleW.h"
#include "AcDbCrossCircle.h"

#define AXEntityDocLockNoDbOk(objId)                        \
  AcAxDocLock docLock(objId, AcAxDocLock::kNormal);       \
  if (docLock.lockStatus() != Acad::eNoDatabase && \
  docLock.lockStatus() != Acad::eOk)                  \
  throw docLock.lockStatus();

#define AXEntityDocLock(objId)                              \
  AcAxDocLock docLock(objId, AcAxDocLock::kNormal);       \
  if(docLock.lockStatus() != Acad::eOk)                   \
  throw docLock.lockStatus();

#define CHECKOUTPARAM(x) if (x==NULL) return E_POINTER;

//CCrCircleW
STDMETHODIMP CCrCircleW::InterfaceSupportsErrorInfo (REFIID riid)
{
	static const IID * arr[] =
	{
		&IID_ICrCircleW
	};

	for (int i=0; i < sizeof(arr)/sizeof(arr[0]); i++ )
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return (S_OK);
	}

  return (S_FALSE);
}

//IAcadBaseObjectImpl
HRESULT CCrCircleW::CreateNewObject (AcDbObjectId &objId, AcDbObjectId &ownerId, TCHAR *keyName)
{
	try
	{
		HRESULT hr;

		if (FAILED(hr = CreateObject(ownerId, keyName)))
		  throw hr ;

		if (FAILED(hr = AddToDb(objId, ownerId, keyName)))
		  throw hr ;
	} 
	catch(HRESULT hr)
	{
		return (hr) ;
	}

	return (S_OK) ;
}

//IAcadBaseObject2Impl
STDMETHODIMP CCrCircleW::ForceDbResident (VARIANT_BOOL *forceDbResident)
{
	if (forceDbResident == NULL )
		return (E_POINTER);

	*forceDbResident = ACAX_VARIANT_FALSE ;
	return (S_OK) ;
}

STDMETHODIMP CCrCircleW::CreateObject(AcDbObjectId ownerId, TCHAR *keyName)
{
	try
	{
		Acad::ErrorStatus   es;
		AcDbObjectPointer<AcDbCrossCircle>   obj;

		if ((es = obj.create()) != Acad::eOk)
		  throw es ;

		obj->setDatabaseDefaults(ownerId.database());

		AcDbCrossCircle *pTmp = NULL;
		obj.release(pTmp);

		SetObject ((AcDbObject *&)pTmp);
	}
	catch ( const Acad::ErrorStatus )
	{
		return (Error (L"Не удалось создать AcDbCrossCircle", IID_ICrCircleW, E_FAIL)) ;
	}

	return (S_OK) ;
}

STDMETHODIMP CCrCircleW::AddToDb (AcDbObjectId &objId, AcDbObjectId ownerId /*=AcDbObjectId::kNull*/, TCHAR *keyName /*=NULL*/)
{
  try
  {
    AcAxDocLock docLock (ownerId) ;

    Acad::ErrorStatus es ;
    AcAxObjectRefPtr<AcDbCrossCircle> obj (&m_objRef, AcDb::kForRead) ;

    //----- Entities can be owned by block table records only
    AcDbBlockTableRecordPointer pBlockTableRecord (ownerId, AcDb::kForWrite, true) ;
    if ( (es =pBlockTableRecord.openStatus ()) != Acad::eOk )
      throw es ;

    if ( (es =pBlockTableRecord->appendAcDbEntity (objId, obj.object ())) != Acad::eOk )
      throw es ;
  }
  catch( const Acad::ErrorStatus )
  {
    return (Error(L"Не удалось добавить AcDbCrossCircle в базу чертежа", IID_ICrCircleW, E_FAIL)) ;
  }
  return (SetObjectId (objId)) ;
}

//IOPMPropertyExpander
STDMETHODIMP CCrCircleW::GetElementValue (DISPID dispID, DWORD dwCookie, VARIANT *pVarOut)
{
  if (pVarOut == NULL)
    return E_POINTER;
  AcAxObjectRefPtr<AcDbCrossCircle> pCrCircle(&m_objRef, AcDb::kForRead);
  if (pCrCircle.openStatus() != Acad::eOk)
    return E_ACCESSDENIED;
  if (dispID == DISPID_CENTER)
  {
    AcGePoint3d acgePt;
    acgePt = pCrCircle->center();
    acdbWcs2Ucs(asDblArray(acgePt),asDblArray(acgePt),Adesk::kFalse);
    AcAxPoint3d acaxPt(acgePt);
    ::VariantCopy(pVarOut,&CComVariant(acaxPt[dwCookie]));
  }
  return S_OK;
}

STDMETHODIMP CCrCircleW::SetElementValue (DISPID dispID, DWORD dwCookie, VARIANT VarIn)
{
  AcAxDocLock docLock(m_objRef.objectId(), AcAxDocLock::kNormal);

  if(docLock.lockStatus() != Acad::eOk && docLock.lockStatus() != Acad::eNoDatabase)
    return E_ACCESSDENIED;

  AcAxObjectRefPtr<AcDbCrossCircle> pCrCircle(&m_objRef, AcDb::kForRead);

  if (pCrCircle.openStatus() != Acad::eOk)
    return E_ACCESSDENIED;

  if (dispID == DISPID_CENTER)
  {
    AcGePoint3d acgePt;
    acgePt = pCrCircle->center();
    acdbEcs2Ucs(asDblArray(acgePt),asDblArray(acgePt),asDblArray(pCrCircle->normal()),Adesk::kFalse);

    AcAxPoint3d acaxPt(acgePt);

    acaxPt[dwCookie] = V_R8(&VarIn);

    acdbUcs2Wcs(asDblArray(acaxPt),asDblArray(acaxPt),Adesk::kFalse);

    pCrCircle->upgradeOpen();
    pCrCircle->setCenter(acaxPt);
    Fire_Notification(DISPID_CENTER);
  }
  return S_OK;
}

STDMETHODIMP CCrCircleW::GetElementStrings (DISPID dispID, OPMLPOLESTR __RPC_FAR *pCaStringsOut, OPMDWORD __RPC_FAR *pCaCookiesOut)
{
  if ( pCaStringsOut == NULL || pCaCookiesOut == NULL )
    return (E_POINTER) ;

  long size;

  if (dispID == DISPID_CENTER)
  {
    size = 2;
    pCaStringsOut->pElems = (LPOLESTR *)::CoTaskMemAlloc(sizeof(LPOLESTR) * size);
    pCaCookiesOut->pElems = (DWORD *)::CoTaskMemAlloc(sizeof(DWORD) * size);
    for (long i=0;i<size;i++)
      pCaCookiesOut->pElems[i] = i;
    pCaStringsOut->cElems = size;
    pCaCookiesOut->cElems = size;
    pCaStringsOut->pElems[0] = ::SysAllocString(L"Центр X");
    pCaStringsOut->pElems[1] = ::SysAllocString(L"Центр Y");
  }
  return S_OK;
}

STDMETHODIMP CCrCircleW::GetElementGrouping (DISPID dispID, short *groupingNumber)
{
  if ( groupingNumber == NULL )
    return (E_POINTER) ;
  //----- Add your code here.

  //return (S_OK) ; //----- If you do anything in there 
  return (E_NOTIMPL) ;
}

STDMETHODIMP CCrCircleW::GetGroupCount (DISPID dispID, long *nGroupCnt)
{
  if ( nGroupCnt == NULL )
    return (E_POINTER) ;
  //----- Add your code here.

  //return (S_OK) ; //----- If you do anything in there 
  return (E_NOTIMPL) ;
}

// Локализуем базовые наименования свойств
STDMETHODIMP CCrCircleW::GetDisplayName (DISPID dispId, BSTR *propName) 
{ 
  switch (dispId) 
  { 
  case (0x401): 
    *propName  = ::SysAllocString(_T("КрестКруг")); 
    break; 

  case (0x516): 
    *propName  = ::SysAllocString(_T("Цвет")); 
    break; 

  case (0x501): 
    *propName  = ::SysAllocString(_T("Слой")); 
    break; 

  case (0x502): 
    *propName  = ::SysAllocString(_T("Тип линий")); 
    break; 

  case (0x503): 
    *propName  = ::SysAllocString(_T("Масштаб типа линий")); 
    break; 

  case (0x513): 
    *propName  = ::SysAllocString(_T("Стиль печати")); 
    break; 

  case (0x514): 
    *propName  = ::SysAllocString(_T("Вес линий")); 
    break; 

  case (0x515): 
    *propName  = ::SysAllocString(_T("Гиперссылка")); 
    break; 

  case (0x577): 
    *propName  = ::SysAllocString(_T("Материал")); 
    break; 

  case (0x02): 
    *propName  = ::SysAllocString(L"Радиус"); 
    break; 

  case (0x03): 
    *propName  = ::SysAllocString(L"Угол"); 
    break; 

  } 
  return S_OK; 
}  

STDMETHODIMP CCrCircleW::get_Radius(double * pVal)
{
  CHECKOUTPARAM(pVal);
  try
  {
    Acad::ErrorStatus es;
    AcAxObjectRefPtr<AcDbCrossCircle> pCrCircle(&m_objRef,AcDb::kForRead,Adesk::kTrue);
    if((es=pCrCircle.openStatus()) != Acad::eOk)
      throw es;

    *pVal = pCrCircle->radius();
  }
  catch(const Acad::ErrorStatus)
  {
    return Error(L"Не удалось открыть объект",IID_ICrCircleW,E_FAIL);
  }
  return S_OK;
}

STDMETHODIMP CCrCircleW::put_Radius(double newVal)
{
  try
  {
    AXEntityDocLockNoDbOk(m_objRef.objectId());

    Acad::ErrorStatus es;
    AcAxObjectRefPtr<AcDbCrossCircle> pCrCircle(&m_objRef,AcDb::kForWrite,Adesk::kTrue);
    if((es=pCrCircle.openStatus()) != Acad::eOk)
      throw es;

    if ((es=pCrCircle->setRadius(newVal))!=Acad::eOk)
      throw es;
    else 
      Fire_Notification(DISPID_RADIUS);
  }
  catch(const Acad::ErrorStatus)
  {
    return Error(L"Не удалось записать радиус.",IID_ICrCircleW,E_FAIL);
  }
  return S_OK;

}

STDMETHODIMP CCrCircleW::get_Center(VARIANT * pVal)
{
  CHECKOUTPARAM(pVal);
  try
  {
    Acad::ErrorStatus es;
    AcAxObjectRefPtr<AcDbCrossCircle> pCrCircle(&m_objRef,AcDb::kForRead,Adesk::kTrue);
    if((es=pCrCircle.openStatus()) != Acad::eOk)
      throw es;
    AcAxPoint3d pt = pCrCircle->center();

    pt.setVariant(pVal);
  }
  catch(const Acad::ErrorStatus)
  {
    return Error(L"Не удалось открыть объект.",IID_ICrCircleW,E_FAIL);
  }
  catch(const HRESULT hr)
  {
    return Error(L"Неверный аргумент.",IID_ICrCircleW,hr);
  }
  return S_OK;

}

STDMETHODIMP CCrCircleW::put_Center(VARIANT newVal)
{
  try
  {
    AcAxPoint3d pt = newVal;
    AXEntityDocLockNoDbOk(m_objRef.objectId());

    Acad::ErrorStatus es;
    AcAxObjectRefPtr<AcDbCrossCircle> pCrCircle(&m_objRef,AcDb::kForWrite,Adesk::kTrue);
    if((es=pCrCircle.openStatus()) != Acad::eOk)
      throw es;

    if ((es=pCrCircle->setCenter(AcGePoint3d(pt.x,pt.y,pt.z)))!=Acad::eOk)
      throw es;
    else
      Fire_Notification(DISPID_CENTER);
  }
  catch(const Acad::ErrorStatus)
  {
    return Error(L"Не удалось присвоить центр круга.",IID_ICrCircleW,E_FAIL);
  }
  catch(const HRESULT hr)
  {
    return Error(L"Неверный аргумент.",IID_ICrCircleW,hr);
  }
  return S_OK;
}

STDMETHODIMP CCrCircleW::get_AngleD(ACAD_ANGLE * pVal)
{
  CHECKOUTPARAM(pVal);
  try
  {
    Acad::ErrorStatus es;
    AcAxObjectRefPtr<AcDbCrossCircle> pCrCircle(&m_objRef,AcDb::kForRead,Adesk::kTrue);
    if((es=pCrCircle.openStatus()) != Acad::eOk)
      throw es;

    *pVal = pCrCircle->angle();
  }
  catch(const Acad::ErrorStatus)
  {
    return Error(L"Не удалось открыть объект",IID_ICrCircleW,E_FAIL);
  }
  return S_OK;
}

STDMETHODIMP CCrCircleW::put_AngleD(ACAD_ANGLE newVal)
{
  try
  {
    AXEntityDocLockNoDbOk(m_objRef.objectId());

    Acad::ErrorStatus es;
    AcAxObjectRefPtr<AcDbCrossCircle> pCrCircle(&m_objRef,AcDb::kForWrite,Adesk::kTrue);
    if((es=pCrCircle.openStatus()) != Acad::eOk)
      throw es;

    if ((es=pCrCircle->setAngle(newVal))!=Acad::eOk)
      throw es;
    else 
      Fire_Notification(DISPID_ANGLE);
  }
  catch(const Acad::ErrorStatus)
  {
    return Error(L"Не удалось присвоить угол.",IID_ICrCircleW,E_FAIL);
  }
  return S_OK;

}
