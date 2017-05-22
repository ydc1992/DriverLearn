#include "main.h"
#include "process.h"
#include "ssdt.h"

UNICODE_STRING g_dev_name = RTL_CONSTANT_STRING(L"\\DEVICE\\MyDriver2");
UNICODE_STRING pSym = RTL_CONSTANT_STRING(L"\\??\\MyDriver2");

PDEVICE_OBJECT pDev = NULL;
PDRIVER_OBJECT CurrentDriverObject = NULL;

ULONG_PTR  SSDTDescriptor = 0;
ULONG_PTR  SSSDTDescriptor = 0;

NTSTATUS DefaultPassThrough(PDEVICE_OBJECT  DeviceObject, PIRP Irp)
{
	Irp->IoStatus.Information = 0;
	Irp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegPath)
{
	NTSTATUS status = STATUS_SUCCESS;

	DbgPrint("DriverEntry\r\n");
	DriverObject->DriverUnload = UnloadDriver;

	status = IoCreateDevice(DriverObject, 0, &g_dev_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &pDev);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("Create Device Faild");
		return status;
	}

	IoDeleteSymbolicLink(&pSym);
	IoCreateSymbolicLink(&pSym, &g_dev_name);

	for (int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = DefaultPassThrough;
	}

	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ControlPassThrough;

	pDev->Flags &= ~DO_DEVICE_INITIALIZING;
	CurrentDriverObject = DriverObject;
	return STATUS_SUCCESS;
}

//************************************
// Method:    ControlPassThrough
// FullName:  ControlPassThrough
// Access:    public
// Returns:   NTSTATUS
// Qualifier: 设备控制分发函数
// Parameter: PDEVICE_OBJECT deviceObj
// Parameter: PIRP irp
//************************************
NTSTATUS ControlPassThrough(PDEVICE_OBJECT deviceObj, PIRP irp) {
	PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(irp);
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ret_len = 0;

	//IRP Params
	PVOID buffer = irp->AssociatedIrp.SystemBuffer;

	ULONG inlen = irpsp->Parameters.DeviceIoControl.InputBufferLength;
	ULONG oulen = irpsp->Parameters.DeviceIoControl.OutputBufferLength;

	ULONG code = IOCTL_GET_MODULE_NAME;

	WCHAR szModuleName[60] = { 0 };
	//
	PVOID SSSDTFunctionAddress = NULL;

	//都需要这个，放在外面
	SSSDTDescriptor = (ULONG_PTR)GetKeShadowServiceDescriptorTable32();

	switch (irpsp->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_HIDE_PROCESS:
		HideProcess((char*)buffer);
		break;
	case IOCTL_GET_SSSDT:
		break;

	case IOCTL_GET_SSSDT_FUNCTIONADDRESS:
	{
		if ((PVOID)SSSDTDescriptor == NULL)
			break;
		if (buffer == NULL)
			break;
		SSSDTFunctionAddress = GetSSSDTFunctionAddrress(*(ULONG*)buffer, SSSDTDescriptor);
		if (SSSDTFunctionAddress != NULL)
		{
			DbgPrint("%x", SSSDTFunctionAddress);
			memcpy(buffer, &SSSDTFunctionAddress, sizeof(PVOID));
			irp->IoStatus.Information = sizeof(PVOID);
			return irp->IoStatus.Status;
		}
		break;
	}
	case IOCTL_GET_MODULE_NAME:
	{
		if ((PVOID)SSSDTDescriptor == NULL)
			break;
		if (buffer == NULL)
			break;
		SSSDTFunctionAddress = GetSSSDTFunctionAddrress(*(ULONG*)buffer, SSSDTDescriptor);

		if ((PVOID)SSSDTDescriptor)
			GetSysModuleByLdrDataTable(SSSDTFunctionAddress, szModuleName);
		break;
	}
	default:
		status = STATUS_INVALID_PARAMETER;
		break;
	}

	irp->IoStatus.Information = 0;
	irp->IoStatus.Status = STATUS_SUCCESS;

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

VOID UnloadDriver(PDRIVER_OBJECT  DriverObject)
{
	ASSERT(pDev != NULL);
	IoDeleteSymbolicLink(&pSym);
	IoDeleteDevice(pDev);
}