
// CPP file fingerprint for future investigations
//
static char s_szFingerprint[]="$RCSfile: HostCA.cpp $ $Revision: 1.5 $";

#include "StdAfx.h"
#include "HostCA.h"
#include "wcautil.h"
#include "strutil.h"
#include "Utils.h"

// as outlined in RFC 4122, this is our namespace for generating name-based (version 3) UUIDs
static WCHAR s_szNamespaceGUID[]=L"D5E06FC6-1D25-4EFD-90A1-57D76BC47395";

int getHosts(const HostFamily &hostFamily, CArray<Host, Host&> &hosts)
{
  CRegKey keyHostRoot, keyHostProduct, keyHost;
  WCHAR szHostProduct[MAX_PATH], szHostVersion[MAX_PATH], szHostName[MAX_PATH], szHostLocation[MAX_PATH], szHostApiVersion[32];
  DWORD nHostName=0, nHostLocation=0, nHostApiVersion=0;
  HRESULT hr;

  hosts.RemoveAll();

  hr=keyHostRoot.Open(HKEY_LOCAL_MACHINE, hostFamily.sHostRoot, KEY_READ | hostFamily.dwKeyFlags);
  if (hr!=S_OK)
    return 0;

  for (int i=0;; i++){
    hr=RegEnumKey(keyHostRoot, i, szHostProduct, _countof(szHostProduct));
    if (hr!=S_OK)
      break;

    hr=keyHostProduct.Open(HKEY_LOCAL_MACHINE, hostFamily.sHostRoot+L"\\"+szHostProduct, KEY_READ | hostFamily.dwKeyFlags);
    if (hr!=S_OK)
      continue;

    for (int j=0;; j++){
      hr=RegEnumKey(keyHostProduct, j, szHostVersion, _countof(szHostVersion));
      if (hr!=S_OK)
        break;

      CString sHostKey=hostFamily.sHostRoot+L"\\"+szHostProduct+L"\\"+szHostVersion;
      hr=keyHost.Open(HKEY_LOCAL_MACHINE, sHostKey, KEY_READ | hostFamily.dwKeyFlags);
      if (hr!=S_OK)
        continue;

      nHostName=sizeof(szHostName)/sizeof(szHostName[0]);
      hr=keyHost.QueryStringValue(_T("ProductName"), szHostName, &nHostName);
      if (hr!=S_OK){
        StringCchPrintf(szHostName, sizeof(szHostName)/sizeof(szHostName[0]), L"%s %s", szHostProduct, szHostVersion);
      }

      szHostLocation[0]=0;
      nHostLocation=sizeof(szHostLocation)/sizeof(szHostLocation[0]);
      hr=keyHost.QueryStringValue(_T("InstallLocation"), szHostLocation, &nHostLocation);

      szHostApiVersion[0]=0;
      nHostApiVersion=sizeof(szHostApiVersion)/sizeof(szHostApiVersion[0]);
      hr=keyHost.QueryStringValue(_T("ApiVersion"), szHostApiVersion, &nHostApiVersion);

      if (szHostApiVersion[0]){
        int nVMajorHost, nVMinorHost, nBMajorHost, nBMinorHost, nBInstHost, nVMajorApp, nVMinorApp, nBMajorApp, nBMinorApp, nBInstApp;
        ParseVersionString(szHostApiVersion, nVMajorHost,  nVMinorHost,  nBMajorHost,  nBMinorHost, nBInstHost);
        ParseVersionString(hostFamily.sApiVersion, nVMajorApp,  nVMinorApp,  nBMajorApp,  nBMinorApp, nBInstApp);
        
        BOOL bHostGoodForApp=FALSE;
        if (nVMajorApp==nVMajorHost && nVMinorApp<=nVMinorHost){
          bHostGoodForApp=TRUE;
        }

        // Process if can parse any string version
        //
        if (bHostGoodForApp){
          Host host;

          host.m_HostProductName=szHostName;
          host.m_HostKey.m_KeyName=sHostKey;
          host.m_HostKey.m_dwKeyFlags=hostFamily.dwKeyFlags;
          host.m_HostDir=szHostLocation;
          host.m_HostExe=L""; // TODO: get host.exe name

          hosts.Add(host);
        }
      }
    }
  }

  return (int)hosts.GetSize();
}

const LPCTSTR vcsSupportedHostsQuery = _T("SELECT * FROM `SupportedHosts`");
enum eSupportedHostsQuery { saqVendorKey = 1, saqHostKey, saqKeyFlags, saqFeature, saqAPIVersion };
const LPCTSTR vcsFeatureComponentsByFeature = _T("SELECT * FROM `FeatureComponents` WHERE `Feature_` = \'%s\'");
enum eFeatureComponentsByFeature { fcfqFeature = 1, fcfqComponent };

#pragma comment(linker, "/EXPORT:PreparePerHostFeaturesCA=_PreparePerHostFeaturesCA@4")
extern "C" UINT __stdcall PreparePerHostFeaturesCA(MSIHANDLE hInstall)
{
  HRESULT hr = S_OK;
  MSIDBERROR dbError;
  UINT er = ERROR_SUCCESS;
  INT iInstalledFeaturesCount = 0;

  LPTSTR pwzData = NULL;

  CArray<Host, Host&> hosts;
  MSIHANDLE hFeatureView = NULL, hFeatureColumns = NULL;
  MSIHANDLE hComponentView = NULL, hComponentColumns = NULL;
  MSIHANDLE hRegistryView = NULL, hRegistryColumns = NULL;
  MSIHANDLE hFCView = NULL, hFCColumns = NULL;
  MSIHANDLE hShortcutView = NULL, hShortcutColumns = NULL;
  MSIHANDLE hListBoxView = NULL, hListBoxColumns = NULL;
  PMSIHANDLE hHostsView = NULL, hHostRec = NULL, hFCRec = NULL;

  CString sProductRegistryKey, sPerHostFeatureDescTemplate, sPerHostComponentDir;
  CString sSupportedHostKey, sSupportedFeature, sSupportedAPIVersion;
  CString sPerHostFeatures, sPerHostFakeGUID, sPerHostFeatureDesc;
  CString sHostIniRegName, sPerHostIniRegGUID, sPerHostAppsRegKey;
  CString sAppPackageProductName, sHostIniFile, sHostIniFileKey;
  DWORD dwSupportedKeyFlags;

  CString sProductName;
  CString sPerHostDirProperty;
  CString sPerHostKeyProperty, sPerHostIniProperty;
  CString sPerHostProfileKeyFlagsProperty;

  // initialize
  hr = WcaInitialize(hInstall, "PreparePerHostFeaturesCA");
  ExitOnFailure(hr, "Failed to initialize PreparePerHostFeaturesCA");

  DWORD_PTR dwFlags;
  IsWow64() ? dwFlags = KEY_WOW64_64KEY | KEY_WOW64_32KEY : dwFlags = 0;

  WcaLog(LOGMSG_STANDARD, "Find host applications with dwFlags = %i", dwFlags);

  // Get necessary properties
  //
  CSCA_GET_PROPERTY_OK_EMPTY(L"ProductNameShort", sProductName);
  if (sProductName.IsEmpty()){
    CSCA_GET_PROPERTY(L"ProductName", sProductName);
  }
  CSCA_GET_PROPERTY(L"ProductRegistryKey", sProductRegistryKey);
  CSCA_GET_PROPERTY(L"PerHostFeatureDescTemplate", sPerHostFeatureDescTemplate);
  sPerHostComponentDir=L"INSTALLLOCATION";

  // Loop through all supported Hosts
  //
  hr = WcaOpenExecuteView(vcsSupportedHostsQuery, &hHostsView);
  ExitOnFailure(hr, "Failed to execute view on SupportedHosts table");

  while (S_OK == (hr = WcaFetchRecord(hHostsView, &hHostRec)))
  {
    hr = WcaGetRecordString(hHostRec, saqHostKey, &pwzData);
    ExitOnFailure(hr, "Failed to get HostKey");
    sSupportedHostKey=pwzData;

    // Set default value if empty
    //
    if(sSupportedHostKey.IsEmpty())
      ExitOnFailure(S_FALSE, "Entry 'HostKey' must be not empty!");

    hr = WcaGetRecordInteger(hHostRec, saqKeyFlags, (int*)&dwSupportedKeyFlags);
    ExitOnFailure(hr, "Failed to get KeyFlags");

    hr = WcaGetRecordString(hHostRec, saqFeature, &pwzData);
    ExitOnFailure(hr, "Failed to get Feature");
    sSupportedFeature=pwzData;

    hr = WcaGetRecordString(hHostRec, saqAPIVersion, &pwzData);
    ExitOnFailure(hr, "Failed to get APIVersion");
    sSupportedAPIVersion=pwzData;

    // Set default value if empty
    //
    if(sSupportedAPIVersion.IsEmpty())
      sSupportedAPIVersion = SUPPORTED_ANY_APIVERSION;

    HostFamily hostFamily;
    hostFamily.sHostRoot=sSupportedHostKey;
    hostFamily.dwKeyFlags=dwSupportedKeyFlags;
    hostFamily.sApiVersion=sSupportedAPIVersion;
    
    int hostsNum=getHosts(hostFamily, hosts);
    hr=WcaSetIntProperty(L"HostsNum", hostsNum);
    WcaLog(LOGMSG_STANDARD, "Found %d host applications with root key '%S'", hostsNum, sSupportedHostKey);

    for (int i=0; i<hostsNum; i++){
      CString sHostFeatureName = hosts[i].m_HostProductName;
      CharUpperBuff(sHostFeatureName.GetBuffer(sHostFeatureName.GetLength()+1), sHostFeatureName.GetLength()+1); // .MakeUpper() is locale-dependent 
      sHostFeatureName.ReleaseBuffer();
      sHostFeatureName.Replace(L':', L'_');
      sHostFeatureName.Replace(L'-', L'_');
      sHostFeatureName.Replace(L'.', L'_');
      sHostFeatureName.Replace(L' ', L'_');

      // Check
      //
      ExitOnNull(!sHostFeatureName.IsEmpty(), hr, E_FAIL, "Failed to create HostKey feature");

      // Fill Host-dependent variables
      //
      sPerHostFeatures+=sHostFeatureName+L" ";
      sPerHostKeyProperty=sHostFeatureName+L"_KEY";                  // Host key property
      sPerHostDirProperty=sHostFeatureName+L"_DIR";                  // Property contains <targetHost>.exe directory
      sPerHostIniProperty=sHostFeatureName+L"_INI";                  // Host ini property
      sPerHostProfileKeyFlagsProperty=sHostFeatureName+L"_KEY_FLAGS";// dwKeyFlags for registry functions

      // Set Host-dependent properties
      //
      hr=WcaSetProperty(sPerHostKeyProperty, hosts[i].m_HostKey.m_KeyName);
      ExitOnFailure1(hr, "Failed to set property %S", sPerHostKeyProperty);
      hr=WcaSetProperty(sPerHostDirProperty, hosts[i].m_HostDir);
      ExitOnFailure1(hr, "Failed to set property %S", sPerHostDirProperty);
      sHostIniFile=sSupportedFeature + L".package";
      hr=WcaSetProperty(sPerHostIniProperty, sHostIniFile);
      ExitOnFailure1(hr, "Failed to set property %S", sPerHostIniProperty);
      CString sKeyFlags;
      sKeyFlags.Format(L"0x%X", hosts[i].m_HostKey.m_dwKeyFlags);
      hr=WcaSetProperty(sPerHostProfileKeyFlagsProperty, sKeyFlags);
      ExitOnFailure1(hr, "Failed to set property %S", sPerHostProfileKeyFlagsProperty);

      // Create per-Host shortcut fake GUID
      //
      sPerHostFakeGUID.Empty();
      hr=getNameBasedGUID(s_szNamespaceGUID, sProductRegistryKey+L"\\"+sHostFeatureName+L"_SHORTCUT", sPerHostFakeGUID);
      AssertSz(!sPerHostFakeGUID.IsEmpty(), "Empty per-Host GUID");
      ExitOnFailure(hr, "Cannot create per-Host GUID");

      WcaLog(LOGMSG_STANDARD, "Preparing per-Host feature %S, %S", sHostFeatureName, sPerHostFakeGUID);

      // Add per-Host feature
      //
      sPerHostFeatureDesc.Format(sPerHostFeatureDescTemplate, hosts[i].m_HostProductName); 

      hr=WcaAddTempRecord(&hFeatureView, &hFeatureColumns, L"Feature", &dbError, 0, 8, 
        sHostFeatureName,                 // Feature
        L"InstallTo",                     // Parent
        hosts[i].m_HostProductName,       // Title
        sPerHostFeatureDesc,              // Description
        11,                               // Display
        1,                                // Level
        L"",                              // Directory
        8                                 // Attributes
        );                      
      ExitOnFailure1(hr, "Failed to add per-Host feature: %s", sHostFeatureName);

      // Add per-Host component
      //
      hr=WcaAddTempRecord(&hComponentView, &hComponentColumns, L"Component", &dbError, 0, 6, 
        sHostFeatureName,                 // Component
        sPerHostFakeGUID,                 // ComponentId
        sPerHostComponentDir,             // Directory_
        msidbComponentAttributesRegistryKeyPath | 
          ((hosts[i].m_HostKey.m_dwKeyFlags & KEY_WOW64_64KEY) ? msidbComponentAttributes64bit : 0), // Attributes
        L"",                              // Condition
        sHostFeatureName                  // KeyPath
        );                      
      ExitOnFailure1(hr, "Failed to add per-Host component: %s", sHostFeatureName);

      // Add per-Host component registry key path
      //
      hr=WcaAddTempRecord(&hRegistryView, &hRegistryColumns, L"Registry", &dbError, 0, 6, 
        sHostFeatureName,                 // Registry
        2,                                // Root, HKLM
        sProductRegistryKey,              // Key
        sHostFeatureName,                 // Name
        sPerHostFakeGUID,                 // Value
        sHostFeatureName                  // Component_
        );                      
      ExitOnFailure1(hr, "Failed to add per-Host component registry key path: %s", sHostFeatureName);

      // Bind per-Host feature and per-Host component
      //
      hr=WcaAddTempRecord(&hFCView, &hFCColumns, L"FeatureComponents", &dbError, 0, 2, 
        sHostFeatureName, // Feature
        sHostFeatureName  // Component
        );                      
      ExitOnFailure1(hr, "Failed to add per-Host feature and component to FeatureComponents: %S", sHostFeatureName);
      if (hFCView){
        ::MsiCloseHandle(hFCView);
        hFCView=NULL;
      }
      if (hFCColumns){
        ::MsiCloseHandle(hFCColumns);
        hFCColumns=NULL;
      }

      // Copy FeatureComponents table
      //
      CString sFeatureComponentsByFeature;

      sFeatureComponentsByFeature.Format(vcsFeatureComponentsByFeature, sSupportedFeature);

      hr = WcaOpenExecuteView(sFeatureComponentsByFeature, &hFCView);
      ExitOnFailure(hr, "Failed to execute view on FeatureComponents table");

      while (S_OK == (hr = WcaFetchRecord(hFCView, &hFCRec)))
      {
        hr = WcaGetRecordString(hFCRec, fcfqComponent, &pwzData);
        ExitOnFailure(hr, "Failed to get key");

        hr=WcaAddTempRecord(&hFCView, &hFCColumns, L"FeatureComponents", &dbError, 0, 2, 
          sHostFeatureName,  // Feature
          pwzData            // Component
          );
        ExitOnFailure(hr, "Failed to add record to FeatureComponents table");
      }

      // Create per-Host application package registration fake GUID
      //
      sHostIniRegName=sHostFeatureName+L"_PACKAGE";

      sPerHostIniRegGUID.Empty();
      hr=getNameBasedGUID(s_szNamespaceGUID, sProductRegistryKey+L"\\"+sHostIniRegName, sPerHostIniRegGUID);
      AssertSz(!sPerHostIniRegGUID.IsEmpty(), "Empty per-Host application package registration GUID");
      ExitOnFailure(hr, "Cannot create per-Host application package registration GUID");

      // Add per-Host application package registration component
      //
      hr=WcaAddTempRecord(&hComponentView, &hComponentColumns, L"Component", &dbError, 0, 6, 
        sHostIniRegName,                  // Component
        sPerHostIniRegGUID,               // ComponentId
        sPerHostComponentDir,             // Directory_
        msidbComponentAttributesRegistryKeyPath | 
          ((hosts[i].m_HostKey.m_dwKeyFlags & KEY_WOW64_64KEY) ? msidbComponentAttributes64bit : 0), // Attributes
        L"",                              // Condition
        sHostIniRegName                   // KeyPath
        );                      
      ExitOnFailure1(hr, "Failed to add per-Host application package registration component: %s", sHostIniRegName);

      // Compute path to .package file and HKLM\Host\Applications\ThisApp key
      //
      sHostIniFileKey.Format(L"[#%s]", sHostIniFile);
  
      CSCA_GET_PROPERTY(APPPACKAGE_PRODUCTNAME, sAppPackageProductName);
      sPerHostAppsRegKey=CString(hosts[i].m_HostKey.m_KeyName)+L"\\Applications\\"+sAppPackageProductName;

      // Add per-Host component registry key path
      //
      hr=WcaAddTempRecord(&hRegistryView, &hRegistryColumns, L"Registry", &dbError, 0, 6, 
        sHostIniRegName,                  // Registry
        2,                                // Root, HKLM
        sPerHostAppsRegKey,               // Key
        L"Package",                       // Name
        sHostIniFileKey,                  // Value
        sHostIniRegName                   // Component_
        );                      
      ExitOnFailure1(hr, "Failed to add per-Host component registry key path: %s", sHostIniRegName);

      // Bind per-Host feature and per-Host component
      //
      hr=WcaAddTempRecord(&hFCView, &hFCColumns, L"FeatureComponents", &dbError, 0, 2, 
        sHostFeatureName,    // Feature
        sHostIniRegName      // Component
        );                      
      ExitOnFailure1(hr, "Failed to add per-Host feature and component to FeatureComponents: %S", sHostIniRegName);
      if (hFCView){
        ::MsiCloseHandle(hFCView);
        hFCView=NULL;
      }
      if (hFCColumns){
        ::MsiCloseHandle(hFCColumns);
        hFCColumns=NULL;
      }


      if (hFCView){
        ::MsiCloseHandle(hFCView);
        hFCView=NULL;
      }
      if (hFCColumns){
        ::MsiCloseHandle(hFCColumns);
        hFCColumns=NULL;
      }

      hr=WcaSetProperty(PROPERTY_PERHOSTFEATURES, sPerHostFeatures);
      ExitOnFailure(hr, "Failed to set property PerHostFeatures");

      hr = S_OK;
      iInstalledFeaturesCount++;
    }
  }

LExit:
  //
  // Clean up
  
  hr=WcaSetIntProperty(PROPERTY_HOSTNUM, iInstalledFeaturesCount);
  
  if (hFeatureView)
    ::MsiCloseHandle(hFeatureView);
  if (hFeatureColumns)
    ::MsiCloseHandle(hFeatureColumns);
  if (hComponentView)
    ::MsiCloseHandle(hComponentView);
  if (hComponentColumns)
    ::MsiCloseHandle(hComponentColumns);
  if (hRegistryView)
    ::MsiCloseHandle(hRegistryView);
  if (hRegistryColumns)
    ::MsiCloseHandle(hRegistryColumns);
  if (hFCView)
    ::MsiCloseHandle(hFCView);
  if (hFCColumns)
    ::MsiCloseHandle(hFCColumns);
  if (hShortcutView)
    ::MsiCloseHandle(hShortcutView);
  if (hShortcutColumns)
    ::MsiCloseHandle(hShortcutColumns);
  if (hListBoxView)
    ::MsiCloseHandle(hListBoxView);
  if (hListBoxColumns)
    ::MsiCloseHandle(hListBoxColumns);

  ReleaseStr(pwzData);

  er = SUCCEEDED(hr) ? ERROR_SUCCESS : ERROR_INSTALL_FAILURE;
  return WcaFinalize(er);
}

LPCTSTR vcsPerHostFeaturesQuery =
  L"SELECT `Feature` FROM `Feature` WHERE `Feature_Parent` = \'InstallTo\'";

enum ePerHostFeaturesQuery { tafqFeature = 1 };

#pragma comment(linker, "/EXPORT:CheckPerHostFeatureNumCA=_CheckPerHostFeatureNumCA@4")
extern "C" UINT __stdcall CheckPerHostFeatureNumCA(MSIHANDLE hInstall)
{
  HRESULT hr = S_OK;
  UINT er = ERROR_SUCCESS;

  LPTSTR pwzData = NULL;
  int nPerHosts=0;

  CString sCAMessageTitleTemplate, sNoHostSelectedMessage, sProductName;

  PMSIHANDLE hFeatureView = NULL, hRec = NULL;

  // initialize
  hr = WcaInitialize(hInstall, "CheckPerHostsCA");
  ExitOnFailure(hr, "Failed to initialize CheckPerHostsCA");

  hr = WcaOpenExecuteView(vcsPerHostFeaturesQuery, &hFeatureView);
  ExitOnFailure1(hr, "Failed to execute view %S", vcsPerHostFeaturesQuery);

  while (S_OK == (hr = WcaFetchRecord(hFeatureView, &hRec)))
  {
    hr = WcaGetRecordString(hRec, tafqFeature, &pwzData);
    ExitOnFailure(hr, "Failed to get Feature name");

    INSTALLSTATE iInstalled, iAction;

    hr=MsiGetFeatureStateW(hInstall, pwzData, &iInstalled, &iAction);
    ExitOnFailure1(hr, "Failed to get feature \"%S\" state", pwzData);

    if ((iInstalled == INSTALLSTATE_LOCAL && iAction!=INSTALLSTATE_ABSENT) || 
      iAction == INSTALLSTATE_LOCAL){
        nPerHosts++;
    }
  }

  hr=WcaSetIntProperty(L"TargetHostsNum", nPerHosts);
  ExitOnFailure(hr, "Failed to set property TargetHostsNum");

  int nUILevel;
  hr=WcaGetIntProperty(L"UILevel", &nUILevel);
  ExitOnFailure(hr, "Failed to get property UILevel");

  CSCA_GET_PROPERTY(L"CAMessageTitleTemplate", sCAMessageTitleTemplate);
  CSCA_GET_PROPERTY(L"NoHostSelectedMessage", sNoHostSelectedMessage);
  CSCA_GET_PROPERTY(L"ProductName", sProductName);

  if (nPerHosts==0){
    if (nUILevel>=4){
      HWND hWnd = ::GetForegroundWindow();

      CString sCAMessageTitle;
      hr=WcaGetFormattedString(sCAMessageTitleTemplate, &pwzData);
      ExitOnFailure1(hr, "failed to get formatted string '%S'", sCAMessageTitleTemplate);
      sCAMessageTitle=pwzData;

      ::MessageBoxW(hWnd, sNoHostSelectedMessage, sCAMessageTitle, MB_OK | MB_ICONSTOP);
    }
    else{
      hr=E_FAIL;
      WcaLogError(hr, "At least one platform must be selected");
      ExitFunction();
    }
  }

  hr = S_OK;

LExit:
  // clean up

  ReleaseStr(pwzData);

  er = SUCCEEDED(hr) ? ERROR_SUCCESS : ERROR_INSTALL_FAILURE;
  return WcaFinalize(er);
}

#pragma comment(linker, "/EXPORT:HostSetupChecksCA=_HostSetupChecksCA@4")
extern "C" UINT __stdcall HostSetupChecksCA(MSIHANDLE hInstall)
{
  HRESULT hr = S_OK;
  UINT er = ERROR_SUCCESS;
  LPTSTR pwzData = NULL;
  CString sPerHostFeatures, sCAMessageTitleTemplate, sNoSupportedHostMessage;
  int nUILevel;

  // initialize
  hr = WcaInitialize(hInstall, "HostSetupChecksCA");
  ExitOnFailure(hr, "Failed to initialize HostSetupChecksCA");

  CSCA_GET_PROPERTY(L"CAMessageTitleTemplate", sCAMessageTitleTemplate);
  hr=WcaGetIntProperty(L"UILevel", &nUILevel);
  ExitOnFailure(hr, "Failed to get property UILevel");

  // Check if any of supported Hosts present
  //
  CSCA_GET_PROPERTY_OK_EMPTY(L"PerHostFeatures", sPerHostFeatures);

  if (sPerHostFeatures.IsEmpty()){
    if (nUILevel>=4){
      CSCA_GET_PROPERTY(L"NoSupportedHostFoundMessage", sNoSupportedHostMessage);

      HWND hWnd = ::GetForegroundWindow();

      CString sCAMessage, sCAMessageTitle;
      hr=WcaGetFormattedString(sCAMessageTitleTemplate, &pwzData);
      ExitOnFailure1(hr, "failed to get formatted string '%S'", sCAMessageTitleTemplate);
      sCAMessageTitle=pwzData;      

      hr=WcaGetFormattedString(sNoSupportedHostMessage, &pwzData);
      ExitOnFailure1(hr, "failed to get formatted string '%S'", sNoSupportedHostMessage);
      sCAMessage=pwzData;

      ::MessageBoxW(hWnd, sCAMessage, sCAMessageTitle, MB_OK | MB_ICONSTOP);
    }

    hr=E_FAIL;
    ExitOnFailure(hr, "No supported platform found");
  }

  CSCA_SET_INT_PROPERTY(L"HOST_SETUP_CHECKS_OK", 1); // Public property to live across UI and Exec sequences

  hr = S_OK;

LExit:
  // clean up

  ReleaseStr(pwzData);

  er = SUCCEEDED(hr) ? ERROR_SUCCESS : ERROR_INSTALL_FAILURE;
  return WcaFinalize(er);
}
