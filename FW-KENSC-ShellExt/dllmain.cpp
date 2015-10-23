// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "ClassFactory.h"
#include <Shlwapi.h>

// {76F76F87-9DA0-48D6-9277-238FCEC133CA}
static const CLSID CLSID_FW_KENSC = 
{ 0x76f76f87, 0x9da0, 0x48d6, { 0x92, 0x77, 0x23, 0x8f, 0xce, 0xc1, 0x33, 0xca } };


long        g_cDllRef   = 0;

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

	if (IsEqualCLSID(CLSID_FW_KENSC, rclsid))
	{
		hr = E_OUTOFMEMORY;

		CClassFactory *pClassFactory = new CClassFactory();
		if (pClassFactory)
		{
			hr = pClassFactory->QueryInterface(riid, ppv);
			pClassFactory->Release();
		}
	}

	return hr;
}

STDAPI DllCanUnloadNow()
{
	return g_cDllRef > 0 ? S_FALSE : S_OK;
}