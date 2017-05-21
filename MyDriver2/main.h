#pragma once

#include <ntddk.h>


#define IOCTL_HIDE_PROCESS \
	(ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,0x911,METHOD_BUFFERED,FILE_WRITE_DATA)

#define IOCTL_GET_SSSDT \
	(ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,0x912,METHOD_BUFFERED,FILE_WRITE_DATA)

#define IOCTL_GET_SSSDT_FUNCTIONADDRESS \
	(ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,0x913,METHOD_BUFFERED,FILE_WRITE_DATA)

#define IOCTL_GET_MODULE_NAME \
	(ULONG)CTL_CODE(FILE_DEVICE_UNKNOWN,0x914,METHOD_BUFFERED,FILE_WRITE_DATA)

VOID UnloadDriver(PDRIVER_OBJECT  DriverObject);
NTSTATUS ControlPassThrough(PDEVICE_OBJECT deviceObj, PIRP irp);