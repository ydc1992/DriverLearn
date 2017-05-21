#pragma once

#include <ntddk.h>


VOID UnloadDriver(PDRIVER_OBJECT  DriverObject);
NTSTATUS myDispatch(PDEVICE_OBJECT deviceObj, PIRP irp);