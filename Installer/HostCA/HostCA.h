// HostCA.h : header file
// 

#pragma once

#define APPPACKAGE_PRODUCTNAME L"APPPACKAGE_PRODUCTNAME"

#define SUPPORTED_ANY_APIVERSION L"0.0.0.0"

#define CSCA_GET_PROPERTY_OK_EMPTY(PropertyName, PropertyVar) \
  hr=WcaGetProperty(PropertyName, &pwzData); \
  ExitOnFailure1(hr, "Failed to get property %S", PropertyName); \
  PropertyVar=pwzData;

#define CSCA_GET_PROPERTY_INIT_EMPTY(PropertyName, PropertyVar, PropertyInitValue) \
  CSCA_GET_PROPERTY_OK_EMPTY(PropertyName, PropertyVar) \
  if (PropertyVar.IsEmpty()){ \
    PropertyVar=PropertyInitValue; \
  }

#define CSCA_GET_PROPERTY(PropertyName, PropertyVar) \
  CSCA_GET_PROPERTY_OK_EMPTY(PropertyName, PropertyVar) \
  AssertSz(!PropertyVar.IsEmpty(), "Empty property value");

#define CSCA_GET_INT_PROPERTY(PropertyName, PropertyVar) \
  hr=WcaGetIntProperty(PropertyName, PropertyVar); \
  ExitOnFailure1(hr, "Failed to get property %S", PropertyName); \

#define CSCA_SET_PROPERTY(PropertyName, PropertyVar) \
  hr=WcaSetProperty(PropertyName, PropertyVar); \
  ExitOnFailure1(hr, "Failed to set property %S", PropertyVar);

#define CSCA_SET_INT_PROPERTY(PropertyName, PropertyVar) \
  hr=WcaSetIntProperty(PropertyName, PropertyVar); \
  ExitOnFailure1(hr, "Failed to set property %S", PropertyVar);

HRESULT getNameBasedGUID(LPCTSTR szThisAppNamespaceGUID, LPCTSTR szPerHostFeatureName, CString &sPerHostFakeGUID);

#define PROPERTY_HOSTNUM L"HostsNum"
#define PROPERTY_PERHOSTFEATURES L"PerHostFeatures"

HRESULT GetListAsArray(LPCTSTR szList, LPCTSTR szDividers, CStringArray &saArray);

struct HostFamily{
  CString sHostRoot;
  DWORD dwKeyFlags;
  CString sApiVersion;
};

struct HostKey{
  HostKey()
  {
    m_dwKeyFlags=0;
  }

  HostKey(const HostKey &p)
  {
    *this=p;
  }
  const HostKey &operator=(const HostKey &p){
    m_KeyName=p.m_KeyName;
    m_dwKeyFlags=p.m_dwKeyFlags;

    return *this;
  }

  CString m_KeyName;
  DWORD m_dwKeyFlags;
};

struct Host{
  Host() 
  {
  }
  Host(const Host &p)
  {
    *this=p;
  }
  const Host &operator=(const Host &p){
    m_HostProductName=p.m_HostProductName;
    m_HostKey=p.m_HostKey;
    m_HostExe=p.m_HostExe;
    m_HostDir=p.m_HostDir;
    //m_CurProfileName=p.m_CurProfileName;
    //m_Profiles.Copy(p.m_Profiles);

    return *this;
  }

  CString m_HostProductName;
  HostKey m_HostKey;
  CString m_HostExe;
  CString m_HostDir;
  //CString m_CurProfileName;
  //CArray<PMHostProfile, PMHostProfile&> m_Profiles;
};