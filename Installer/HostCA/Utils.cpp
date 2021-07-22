// Utils.cpp : implementation file
//
#include "StdAfx.h"
#include "Utils.h"

extern "C" {
#include "sysdep.h"
#include "uuid.h"
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleA("kernel32"),
                                                                           "IsWow64Process"); 
// Return TRUE  if under WOW64
// Return FALSE if under Win32 or Win64
//
bool IsWow64()
{
  BOOL bIsWow64 = FALSE;

  if (NULL != fnIsWow64Process)
  {
    if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
    {
      return false;
    }
  }
  return bIsWow64==TRUE;
}

// Version utility functions
// 
BOOL ParseVersionString(const WCHAR* szVersion, INT_PTR& nVersionMajor, INT_PTR& nVersionMinor, INT_PTR& nBuildMajor, INT_PTR& nBuildMinor, INT_PTR& nInstallerBuild)
{
  size_t nLen=_tcslen(szVersion);
  WCHAR *szNextToken=NULL;

  // Init
  //
  nVersionMajor = nBuildMajor = nBuildMinor = nInstallerBuild = 0;

  if (nLen==0)
    return FALSE;

  WCHAR *szVersionCopy=new WCHAR[nLen+1], *szTok=0;
  if (!szVersionCopy)
    return FALSE;

  // Create a copy of szVersion before calling wcstok
  //
  _tcscpy_s(szVersionCopy, nLen+1, szVersion);

  szTok = wcstok_s(szVersionCopy, L".", &szNextToken);
  nVersionMajor = szTok ? _ttoi(szTok) : 0;

  szTok = wcstok_s(NULL, L".", &szNextToken);
  nVersionMinor = szTok ? _ttoi(szTok) : 0;

  szTok = wcstok_s(NULL, L".", &szNextToken);
  nBuildMajor = szTok ? _ttoi(szTok) : 0;

  szTok = wcstok_s(NULL, L".", &szNextToken);
  nBuildMinor = szTok ? _ttoi(szTok) : 0;

  szTok = wcstok_s(NULL, L".", &szNextToken);
  nInstallerBuild = szTok ? _ttoi(szTok) : 0;

  delete[] szVersionCopy;

  return TRUE;
}

// Return TRUE if Version2 > Version1
// Set bIncludeEqual to TRUE for "min requirements" feature
BOOL Version2IsGreater(int nVMajor1, int nVMinor1, int nBMajor1, int nBMinor1, int nBInst1,
                       int nVMajor2, int nVMinor2, int nBMajor2, int nBMinor2, int nBInst2, BOOL bIncludeEqual)
{
  BOOL bResult=FALSE;

  if(!bIncludeEqual)
  {
    // Not include equal versions (etc minimal requirements)
    //
    if(nVMajor2 > nVMajor1) { bResult = TRUE; }
    else if ((nVMajor2 == nVMajor1) && (nVMinor2 > nVMinor1)) { bResult = TRUE; }
    else if ((nVMajor2 == nVMajor1) && (nVMinor2 == nVMinor1) && (nBMajor2 > nBMajor1)) { bResult = TRUE; }
    else if ((nVMajor2 == nVMajor1) && (nVMinor2 == nVMinor1) && (nBMajor2 == nBMajor1) && (nBMinor2 > nBMinor1)) { bResult = TRUE; }
    else if ((nVMajor2 == nVMajor1) && (nVMinor2 == nVMinor1) && (nBMajor2 == nBMajor1) && (nBMinor2 == nBMinor1) && (nBInst2 > nBInst1)) { bResult = TRUE; }
  }
  else
  {
    // Special for min requirements
    // 
    if(nVMajor2 > nVMajor1) { bResult = TRUE; }
    else if ((nVMajor2 == nVMajor1) && (nVMinor2 >= nVMinor1)) { bResult = TRUE; }
    else if ((nVMajor2 == nVMajor1) && (nVMinor2 == nVMinor1) && (nBMajor2 >= nBMajor1)) { bResult = TRUE; }
    else if ((nVMajor2 == nVMajor1) && (nVMinor2 == nVMinor1) && (nBMajor2 == nBMajor1) && (nBMinor2 >= nBMinor1)) { bResult = TRUE; }
    else if ((nVMajor2 == nVMajor1) && (nVMinor2 == nVMinor1) && (nBMajor2 == nBMajor1) && (nBMinor2 == nBMinor1) && (nBInst2 >= nBInst1)) { bResult = TRUE; }
  }

  return bResult;
}

// Return TRUE if Version2 > Version1
// Set bIncludeEqual to TRUE for "min requirements" feature
BOOL Version2IsGreater(const WCHAR* szVersion1, const WCHAR* szVersion2, BOOL bIncludeEqual)
{
  int nVMajor1,  nVMinor1,  nBMajor1,  nBMinor1,  nBInst1,
      nVMajor2,  nVMinor2,  nBMajor2,  nBMinor2,  nBInst2;
  BOOL bParseResult = FALSE;

  // Process if can parse any string version
  //
  if((bParseResult |= ParseVersionString(szVersion1, nVMajor1,  nVMinor1,  nBMajor1,  nBMinor1, nBInst1), bParseResult |= ParseVersionString(szVersion2, nVMajor2,  nVMinor2,  nBMajor2,  nBMinor2,  nBInst2), bParseResult))
    return Version2IsGreater(nVMajor1,  nVMinor1,  nBMajor1,  nBMinor1,  nBInst1,
                             nVMajor2,  nVMinor2,  nBMajor2,  nBMinor2,  nBInst2, bIncludeEqual);

  return FALSE;
}

HRESULT getNameBasedGUID(LPCTSTR szNamespaceGUID, LPCTSTR szPerHostFeatureName, CString &sPerHostFakeGUID)
{
  CStringA sNamespaceGUID, sPerHostFeatureName=CW2A(szPerHostFeatureName);
  uuid_t uuidNamespaceGUID;

  sNamespaceGUID=CW2A(szNamespaceGUID);
  sNamespaceGUID.Replace("{", "");
  sNamespaceGUID.Replace("}", "");

  int parsed=uuid_parse(sNamespaceGUID, &uuidNamespaceGUID);
  if (parsed!=11){
    WcaLog(LOGMSG_STANDARD, "Cannot parse PerHostNamespaceGUID %s", szNamespaceGUID);
    return S_FALSE;
  }

  uuid_t uuidPerHostFakeGUID;
  uuid_create_md5_from_name(&uuidPerHostFakeGUID, uuidNamespaceGUID, (void *)(const char *)sPerHostFeatureName, sPerHostFeatureName.GetLength());

  CStringA sPerHostFakeGUIDA;
  uuid_unparse(&uuidPerHostFakeGUID, sPerHostFakeGUIDA.GetBuffer(GUID_BUFFER_LEN));
  sPerHostFakeGUIDA.ReleaseBuffer();

  sPerHostFakeGUID.Format(L"{%S}", (LPCSTR)sPerHostFakeGUIDA);
  return S_OK;
}
