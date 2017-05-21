#include "main.h"
#include "process.h"

UNICODE_STRING g_dev_name = RTL_CONSTANT_STRING(L"\\DEVICE\\Ken");
PDEVICE_OBJECT pDev = NULL;
UNICODE_STRING pSym = RTL_CONSTANT_STRING(L"\\??\\Ken");


#define CTRL_HIDE_PROCESS \
	(ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,0x911,METHOD_BUFFERED,FILE_WRITE_DATA)




NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegPath) 
{
	NTSTATUS status = STATUS_SUCCESS;

	DbgPrint("DriverEntry\r\n");
	DriverObject->DriverUnload = UnloadDriver;

	status =  IoCreateDevice(DriverObject, 0, &g_dev_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDev);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("Create Device Faild");
		return status;
	}

	IoDeleteSymbolicLink(&pSym);
	IoCreateSymbolicLink(&pSym, &g_dev_name);

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = myDispatch;
	}
	return STATUS_SUCCESS;
}


NTSTATUS myDispatch(PDEVICE_OBJECT deviceObj, PIRP irp) {
	PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(irp);
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ret_len = 0;

	while (deviceObj == pDev)
	{

		if (irpsp->MajorFunction==IRP_MJ_CREATE || irpsp->MajorFunction==IRP_MJ_CLOSE)
			break;

		if (irpsp->MajorFunction==IRP_MJ_DEVICE_CONTROL)
		{
			PVOID buffer = irp->AssociatedIrp.SystemBuffer;

			ULONG inlen = irpsp->Parameters.DeviceIoControl.InputBufferLength;
			ULONG oulen = irpsp->Parameters.DeviceIoControl.OutputBufferLength;

			switch (irpsp->Parameters.DeviceIoControl.IoControlCode)
			{
			case CTRL_HIDE_PROCESS:
				break;
			}


		}


	}


	return STATUS_SUCCESS;
}


VOID UnloadDriver(PDRIVER_OBJECT  DriverObject)
{
	ASSERT(pDev != NULL);
	IoDeleteSymbolicLink(&pSym);
	IoDeleteDevice(pDev);
}
