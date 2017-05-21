
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


typedef struct _KLDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY32 InLoadOrderLinks;
	ULONG reaver;
	ULONG reaver1;
	ULONG reaver2;
	ULONG NonPageDebugInfo;
	ULONG DllBase;
	ULONG EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING32 FullDllName;
	UNICODE_STRING32 BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	ULONG reaver3;
	ULONG reaver4;
	ULONG checkSum;
	ULONG TimeDateStamp;
}KLDR_DATA_TABLE_ENTRY,*PKLDR_DATA_TABLE_ENTRY;


WIN_VERSION GetWindowsVersion();
PVOID GetFunctionAddressByNameFromNtosExport(WCHAR *wzFunctionName);