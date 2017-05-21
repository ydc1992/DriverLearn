
#pragma once
#include <ntddk.h>


typedef enum WIN_VERSION {
	WINDOWS_UNKNOW,
	WINDOWS_XP,
	WINDOWS_7,
	WINDOWS_8,
	WINDOWS_8_1
} WIN_VERSION;

typedef struct _SYSTEM_SERVICE_TABLE {
	PVOID ServiceTableBase;
	PVOID ServiceConunterTableBase;
	ULONG32 NumberOfServices;
	PVOID ParamTable;
}SYSTEM_SERVICE_TABLE,*PSYSTEM_SERVICE_TABLE;

WIN_VERSION GetWindowsVersion();
PVOID GetFunctionAddressByNameFromNtosExport(WCHAR *wzFunctionName);