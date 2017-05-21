#include "common.h"

typedef NTSTATUS(*pfnRtlGetVersion)(PRTL_OSVERSIONINFOW lpVersionInformation);


PVOID GetFunctionAddressByNameFromNtosExport(WCHAR *wzFunctionName)
{
	UNICODE_STRING uniFunctionName;
	PVOID FunctionAddress = NULL;

	if (wzFunctionName && wcslen(wzFunctionName) > 0)
	{
		RtlInitUnicodeString(&uniFunctionName, wzFunctionName);
		FunctionAddress = MmGetSystemRoutineAddress(&uniFunctionName);
	}

	return FunctionAddress;
}

//************************************
// Method:    GetWindowsVersion
// FullName:  GetWindowsVersion
// Access:    public 
// Returns:   WIN_VERSION
// Qualifier: 获取系统版本号
//************************************
WIN_VERSION GetWindowsVersion()
{
	RTL_OSVERSIONINFOEXW osverInfo = { sizeof(osverInfo) };
	WIN_VERSION WinVersion;

	pfnRtlGetVersion RtlGetVersion = GetFunctionAddressByNameFromNtosExport(L"RtlGetVersion");
	if (RtlGetVersion)
	{
		RtlGetVersion((PRTL_OSVERSIONINFOW)&osverInfo);
	}
	else
	{
		PsGetVersion(&osverInfo.dwMajorVersion, &osverInfo.dwMinorVersion, &osverInfo.dwBuildNumber, NULL);   //Documet
	}

	DbgPrint("[Windows Version] Build Number: %d\r\n", osverInfo.dwBuildNumber);

	if (osverInfo.dwMajorVersion == 5 && osverInfo.dwMinorVersion == 1)
	{
		DbgPrint("[Windows Version] WINDOWS_XP\r\n");
		WinVersion = WINDOWS_XP;
	}
	else if (osverInfo.dwMajorVersion == 6 && osverInfo.dwMinorVersion == 1)
	{
		DbgPrint("[Windows Version] WINDOWS 7\r\n");
		WinVersion = WINDOWS_7;
	}
	else if (osverInfo.dwMajorVersion == 6 &&
		osverInfo.dwMinorVersion == 2 &&
		osverInfo.dwBuildNumber == 9200)
	{
		DbgPrint("[Windows Version] WINDOWS 8\r\n");
		WinVersion = WINDOWS_8;
	}
	else if (osverInfo.dwMajorVersion == 6 &&
		osverInfo.dwMinorVersion == 3 &&
		osverInfo.dwBuildNumber == 9600)
	{
		DbgPrint("[Windows Version] WINDOWS 8.1\r\n");
		WinVersion = WINDOWS_8_1;
	}
	else
	{
		DbgPrint("[Windows Version] WINDOWS_UNKNOW\r\n");
		WinVersion = WINDOWS_UNKNOW;
	}

	return WinVersion;
}