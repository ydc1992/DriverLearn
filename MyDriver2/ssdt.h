#pragma once
#include "common.h"

PVOID GetSSDTFunctionAddress(ULONG_PTR uIndex, ULONG_PTR SSDTDescriptor);
PVOID GetSSSDTFunctionAddrress(ULONG_PTR uIndex, ULONG_PTR SSDTDescriptor);

PVOID GetKeShadowServiceDescriptorTable32();

BOOLEAN GetSysModuleByLdrDataTable(PVOID addre, WCHAR* szModuleName);