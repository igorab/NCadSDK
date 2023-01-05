#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
//- DLL Entry Point
extern "C"
BOOL WINAPI DllMain (HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved) ;

	if ( dwReason == DLL_PROCESS_ATTACH )
	{
		_hdllInstance = hInstance ;
	}
	else if ( dwReason == DLL_PROCESS_DETACH )
	{
	}

	return (TRUE);
}

