#include "common.h"

extern PDRIVER_OBJECT CurrentDriverObject;
//************************************
// Method:    GetSSDTFunctionAddress
// FullName:  GetSSDTFunctionAddress
// Access:    public 
// Returns:   PVOID
// Qualifier: 通过索引获取从SSDT获取对应函数地址
// Parameter: ULONG_PTR uIndex
// Parameter: ULONG_PTR SSDTDescriptor
//************************************
PVOID GetSSDTFunctionAddress(ULONG_PTR uIndex, ULONG_PTR SSDTDescriptor)
{
	ULONG_PTR ServicesTableBase = 0;
	PSYSTEM_SERVICE_TABLE SSSDT = (PSYSTEM_SERVICE_TABLE)SSDTDescriptor;

	ServicesTableBase = (ULONG_PTR)(SSSDT->ServiceTableBase);
	return (PVOID)(*(PULONG_PTR)((ULONG_PTR)ServicesTableBase + 4 * uIndex));
}

//************************************
// Method:    GetSSSDTFunctionAddrress
// FullName:  GetSSSDTFunctionAddrress
// Access:    public 
// Returns:   PVOID
// Qualifier: 通过索引获取从SSDT获取对应函数地址
// Parameter: ULONG_PTR uIndex
// Parameter: ULONG_PTR SSDTDescriptor
//************************************
PVOID GetSSSDTFunctionAddrress(ULONG_PTR uIndex, ULONG_PTR SSDTDescriptor)
{
	ULONG_PTR ServicesTableBase = 0;
	PSYSTEM_SERVICE_TABLE SSSDT = (PSYSTEM_SERVICE_TABLE)SSDTDescriptor;
	ServicesTableBase = (ULONG_PTR)(SSSDT->ServiceTableBase);
	return (PVOID)(*(PULONG_PTR)((ULONG_PTR)ServicesTableBase + 4 * uIndex));
}


//************************************
// Method:    GetKeShadowServiceDescriptorTable32
// FullName:  GetKeShadowServiceDescriptorTable32
// Access:    public 
// Returns:   PVOID
// Qualifier: lea     ecx,nt!KeServiceDescriptorTableShadow这里存放着SSDT
//************************************
PVOID GetKeShadowServiceDescriptorTable32()
{
	ULONG_PTR SSSDTDescriptor = 0;
	PUCHAR i = 0;
	PUCHAR StartAddress;
	UCHAR v1 = 0, v2 = 0;
	/*
	kd> u KeAddSystemServiceTable l 50
	nt!KeAddSystemServiceTable:
	805b3ff1 8bff            mov     edi,edi
	805b3ff3 55              push    ebp
	805b3ff4 8bec            mov     ebp,esp
	805b3ff6 837d1803        cmp     dword ptr [ebp+18h],3
	805b3ffa 774e            ja      nt!KeAddSystemServiceTable+0x6b (805b404a)
	805b3ffc 8b4518          mov     eax,dword ptr [ebp+18h]
	805b3fff c1e004          shl     eax,4
	805b4002 83b82035568000  cmp     dword ptr nt!KeServiceDescriptorTable (80563520)[eax],0
	805b4009 753f            jne     nt!KeAddSystemServiceTable+0x6b (805b404a)
	805b400b 8d88e0345680    lea     ecx,nt!KeServiceDescriptorTableShadow (805634e0)[eax]
	*/
	StartAddress = (PUCHAR)GetFunctionAddressByNameFromNtosExport(L"KeAddSystemServiceTable");

	if (StartAddress == NULL)
	{
		return 0;
	}

	for (i = StartAddress; i<StartAddress + PAGE_SIZE; i++)
	{
		//if (MmIsAddressValid(i) && MmIsAddressValid(i + 1))
		v1 = *i;
		v2 = *(i + 1);
		if (v1 == 0x8d && v2 == 0x88) 
		{
			SSSDTDescriptor = *(ULONG_PTR*)(i + 2);
			SSSDTDescriptor = SSSDTDescriptor + 16;

			return (PVOID)SSSDTDescriptor;
		}
	}
	return 0;
}

BOOLEAN GetSysModuleByLdrDataTable(PVOID addre, WCHAR* szModuleName) {


	ULONG_PTR ulBase;
	ULONG ulSize;

	if (CurrentDriverObject)
	{
		PKLDR_DATA_TABLE_ENTRY ListHead = NULL, ListNext = NULL;
		ListHead = ListNext = (PKLDR_DATA_TABLE_ENTRY)CurrentDriverObject->DriverSection;

		while ((PKLDR_DATA_TABLE_ENTRY)ListNext->InLoadOrderLinks.Flink != ListHead)
		{
			ulBase = ListNext->DllBase;
			ulSize = ListNext->SizeOfImage;
			BOOLEAN bRet = FALSE;

			if (ulBase != 0)
			{
				_try
				{
					DbgPrint("[SSDT]%wZ\r\n",&ListNext->BaseDllName);
					DbgPrint("[SSDT]%wZ\r\n",&(ListNext->FullDllName));

					memcpy(szModuleName,(WCHAR*)(((ListNext)->FullDllName).Buffer),sizeof(WCHAR) * 60);

				}
				_except(EXCEPTION_EXECUTE_HANDLER)
				{
					DbgPrint("EXCEEPTION:%d", GetExceptionCode());
				}
				bRet = TRUE;
				break;

			}

			ListNext = (PKLDR_DATA_TABLE_ENTRY)ListNext->InLoadOrderLinks.Flink;
		}

	}

	return TRUE;
}