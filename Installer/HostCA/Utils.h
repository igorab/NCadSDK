#pragma once

#define GUID_BUFFER_LEN 39

bool IsWow64();
CString GetMD5Digest(CString &csBuffer);

BOOL ParseVersionString(const WCHAR* szVersion, INT_PTR& nVersionMajor, INT_PTR& nVersionMinor, INT_PTR& nBuildMajor, INT_PTR& nBuildMinor, INT_PTR& nInstallerBuild);