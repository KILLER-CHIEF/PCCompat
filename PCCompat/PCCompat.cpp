// PCCompat.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PCCompat.h"

// #1
int __stdcall _PCC_Evaluate___()
{
	return 0;
}

// #2
int __stdcall _PCC_GetBooleanProperty___(wchar_t* property_name, int *rtn_value)
{
	//MessageBoxW(0, property_name, L"PCC_GetBooleanProperty", MB_OK);
	if (!rtn_value)
		return 0x80004003;

	if (wcscmp(L"VideoDeviceSupportsHardwareTnL", property_name) == 0)
		*rtn_value = 1;
	else if (wcscmp(L"CinematicShadow", property_name) == 0)
		*rtn_value = 1;
	else if (wcscmp(L"AllowVsync", property_name) == 0)
		*rtn_value = 0;
	else
		*rtn_value = 0;

	return 0;
}

// #3
int __stdcall _PCC_GetFloatProperty___(wchar_t* property_name, double *rtn_value)
{
	//MessageBoxW(0, property_name, L"PCC_GetFloatProperty", MB_OK);
	if (!rtn_value)
		return 0x80004003;

	if (wcscmp(L"CPUScore", property_name) == 0)
		*rtn_value = 6.9;
	else if (wcscmp(L"D3DScore", property_name) == 0)
		*rtn_value = 7.7;
	else if (wcscmp(L"DiskScore", property_name) == 0)
		*rtn_value = 5.9;
	else if (wcscmp(L"GraphicsScore", property_name) == 0)
		*rtn_value = 7.7;
	else if (wcscmp(L"MemoryScore", property_name) == 0)
		*rtn_value = 7.0;
	else if (wcscmp(L"SystemScore", property_name) == 0)
		*rtn_value = 5.9;
	else
		*rtn_value = 0.0;

	return 0;
}

// #4
int __stdcall _PCC_GetIntegerProperty___(wchar_t* property_name, int *rtn_value)
{
	//MessageBoxW(0, property_name, L"PCC_GetIntegerProperty", MB_OK);
	if (!rtn_value)
		return 0x80004003;

	if (wcscmp(L"VideoDeviceSupportedPixelShaderVersion", property_name) == 0)
		*rtn_value = 3;
	else if (wcscmp(L"SystemMemory", property_name) == 0)
		*rtn_value = 0x1FC0;
	else if (wcscmp(L"CpuMaxSpeed", property_name) == 0)
		*rtn_value = 0xBFB;
	else if (wcscmp(L"CpuLogicalCores", property_name) == 0)
		*rtn_value = 4;
	else
		*rtn_value = 0;

	return 0;
}

// #5
int __stdcall _PCC_GetMessage___(int a1, wchar_t *Src) {
	if (!Src)
		return 0x80070057;
	*(DWORD*)Src = 0;
	return 0;
}

// #6
signed int __stdcall _PCC_GetMessageCount___(int *a1) {
	signed int result = 0;
	*a1 = 0;
	return result;
}

// #7
signed int __stdcall _PCC_GetStringProperty___(int a1, DWORD *a2)
{
	int v3 = 0;
	if (!a2)
		return 0x80004003;
	*a2 = v3;
	return 0;
}

// #8
signed int __stdcall _PCC_GetVersionProperty___(char a1, DWORD *a2)
{
	int v3 = 0;
	if (!a2)
		return 0x80004003;
	*a2 = v3;
	return 0;
}

// #9
int __stdcall _PCC_Initialize___(int a1, void *Src, void *a3, int a4, int a5, int a6)
{
	return 0;
}

// #10
int __stdcall _PCC_ResetState___()
{
	return 0;
}

// #11
signed int __stdcall _PCC_SetBooleanProperty___(char a1, int a2)
{
	signed int result = 0;
	//result = 0x80004005;
	
	return result;
}

// #12
int __stdcall _PCC_SetFloatProperty___(int a1, double a2)
{
	int result = 0;
	//result = 0x80004005;

	return result;
}

// #13
signed int __stdcall _PCC_SetIntegerProperty___(char a1, int a2)
{
	signed int result = 0;
	//result = 0x80004005;

	return result;
}

// #14
signed int __stdcall _PCC_SetStringProperty___(char a1, void *Src)
{
	signed int result = 0;
	//result = 0x80004005;

	return result;
}

// #15
signed int __stdcall _PCC_SetVersionProperty___(char a1, int a2, int a3, int a4, int a5)
{
	//return 0x80004005;
	return 0;
}

// #16
int __stdcall _PCC_SetVinceOptions___(int a1, int a2, void *Src)
{
	return 0;
}

// #17
int __stdcall _PCC_ShowConsentUI___(HWND hWnd, int a2)
{
	if (!a2)
		return 0x80004003;
	*(DWORD *)a2 = true ? 1 : 0;//yes selected in dialog box.
	return 0;
}

// #18
int __stdcall _PCC_ShowMessages___(HWND a1, DWORD *a2)
{
	if (a2)
		*a2 = 1;
	return 0;
}

// #19
bool __stdcall _PCC_ShutdownCheckpoint___()
{
	//return 0;
	return 1;
}

// #20
int __stdcall _PCC_StartupCheckpoint___()
{
	return 0;
}

// #21
int __stdcall _PCC_Uninitialize___()
{
	return 0;
}
