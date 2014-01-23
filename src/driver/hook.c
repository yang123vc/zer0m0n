////////////////////////////////////////////////////////////////////////////
//
//	zer0m0n DRIVER
//
//  Copyright 2013 Conix Security, Nicolas Correia, Adrien Chevalier
//
//  This file is part of zer0m0n.
//
//  Zer0m0n is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Zer0m0n is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Zer0m0n.  If not, see <http://www.gnu.org/licenses/>.
//
//
//	File :		hook.c
//	Abstract :	SSDT hooks handling
//	Revision : 	v1.0
//	Author :	Adrien Chevalier & Nicolas Correia
//	Email :		adrien.chevalier@conix.fr nicolas.correia@conix.fr
//	Date :		2013-12-26
//	Notes : 
//		
////////////////////////////////////////////////////////////////////////////
#include "hook.h"
#include "main.h"
#include "utils.h"
#include "monitor.h"
#include "comm.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Installs SSDT hooks.
//	Parameters :
//		None
//	Return value :
//		None
//	Process :
//		Unset WP bit from CR0 register to be able to modify SSDT entries, restores the original values,
//		and sets WP bit again.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID unhook_ssdt_entries()
{
	disable_cr0();

	if(oldZwCreateThread != NULL)
		(ZWCREATETHREAD)SYSTEMSERVICE(CREATETHREAD_INDEX) = oldZwCreateThread;
	
	if(oldZwMapViewOfSection != NULL)
		(ZWMAPVIEWOFSECTION)SYSTEMSERVICE(MAPVIEWOFSECTION_INDEX) = oldZwMapViewOfSection;
	
	if(oldZwSetContextThread != NULL)
		(ZWSETCONTEXTTHREAD)SYSTEMSERVICE(SETCONTEXTTHREAD_INDEX) = oldZwSetContextThread;
	
	if(oldZwQueueApcThread != NULL)
		(ZWQUEUEAPCTHREAD)SYSTEMSERVICE(QUEUEAPCTHREAD_INDEX) = oldZwQueueApcThread;
	
	if(oldZwSystemDebugControl != NULL)
		(ZWSYSTEMDEBUGCONTROL)SYSTEMSERVICE(SYSTEMDEBUGCONTROL_INDEX) = oldZwSystemDebugControl;
	
	if(oldZwCreateProcess != NULL)
		(ZWCREATEPROCESS)SYSTEMSERVICE(CREATEPROCESS_INDEX) = oldZwCreateProcess;
	
	if(oldZwCreateProcessEx != NULL)
		(ZWCREATEPROCESSEX)SYSTEMSERVICE(CREATEPROCESSEX_INDEX) = oldZwCreateProcessEx;
	
	if(oldZwWriteVirtualMemory != NULL)
		(ZWWRITEVIRTUALMEMORY)SYSTEMSERVICE(WRITEVIRTUALMEMORY_INDEX) = oldZwWriteVirtualMemory;
	
	if(oldZwDebugActiveProcess != NULL)
		(ZWDEBUGACTIVEPROCESS)SYSTEMSERVICE(DEBUGACTIVEPROCESS_INDEX) = oldZwDebugActiveProcess;
	
	if(oldZwOpenProcess != NULL)
		(ZWOPENPROCESS)SYSTEMSERVICE(OPENPROCESS_INDEX) = oldZwOpenProcess;
	
	if(oldZwOpenThread != NULL)
		(ZWOPENTHREAD)SYSTEMSERVICE(OPENTHREAD_INDEX) = oldZwOpenThread;
	
	if(oldZwQuerySystemInformation != NULL)
		(ZWQUERYSYSTEMINFORMATION)SYSTEMSERVICE(QUERYSYSTEMINFORMATION_INDEX) = oldZwQuerySystemInformation;
	
	if(oldZwCreateFile != NULL)
		(ZWCREATEFILE)SYSTEMSERVICE(CREATEFILE_INDEX) = oldZwCreateFile;
	
	if(oldZwReadFile != NULL)
		(ZWREADFILE)SYSTEMSERVICE(READFILE_INDEX) = oldZwReadFile;
	
	if(oldZwWriteFile != NULL)
		(ZWWRITEFILE)SYSTEMSERVICE(WRITEFILE_INDEX) = oldZwWriteFile;
	
	if(oldZwDeleteFile != NULL)
		(ZWDELETEFILE)SYSTEMSERVICE(DELETEFILE_INDEX) = oldZwDeleteFile;
	
	if(oldZwSetInformationFile != NULL)
		(ZWSETINFORMATIONFILE)SYSTEMSERVICE(SETINFORMATIONFILE_INDEX) = oldZwSetInformationFile;
	
	if(oldZwQueryInformationFile != NULL)
		(ZWQUERYINFORMATIONFILE)SYSTEMSERVICE(QUERYINFORMATIONFILE_INDEX) = oldZwQueryInformationFile;
	
	if(oldZwCreateMutant != NULL)
		(ZWCREATEMUTANT)SYSTEMSERVICE(CREATEMUTANT_INDEX) = oldZwCreateMutant;
		
	enable_cr0();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Installs SSDT hooks.
//	Parameters :
//		None
//	Return value :
//		None
//	Process :
//		Unset WP bit from CR0 register to be able to modify SSDT entries, patch with our values after,
//		saving the original ones, and set the WP bit again.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID hook_ssdt_entries()
{
	disable_cr0();
	
	oldZwCreateThread = (ZWCREATETHREAD)SYSTEMSERVICE(CREATETHREAD_INDEX);
	(ZWCREATETHREAD)SYSTEMSERVICE(CREATETHREAD_INDEX) = newZwCreateThread;
	
	oldZwSetContextThread = (ZWSETCONTEXTTHREAD)SYSTEMSERVICE(SETCONTEXTTHREAD_INDEX);
	(ZWSETCONTEXTTHREAD)SYSTEMSERVICE(SETCONTEXTTHREAD_INDEX) = newZwSetContextThread;
	
	oldZwQueueApcThread = (ZWQUEUEAPCTHREAD)SYSTEMSERVICE(QUEUEAPCTHREAD_INDEX);
	(ZWQUEUEAPCTHREAD)SYSTEMSERVICE(QUEUEAPCTHREAD_INDEX) = newZwQueueApcThread;
	
	oldZwWriteVirtualMemory = (ZWWRITEVIRTUALMEMORY)SYSTEMSERVICE(WRITEVIRTUALMEMORY_INDEX);
	(ZWWRITEVIRTUALMEMORY)SYSTEMSERVICE(WRITEVIRTUALMEMORY_INDEX) = newZwWriteVirtualMemory;
	
	oldZwSystemDebugControl = (ZWSYSTEMDEBUGCONTROL)SYSTEMSERVICE(SYSTEMDEBUGCONTROL_INDEX);
	(ZWSYSTEMDEBUGCONTROL)SYSTEMSERVICE(SYSTEMDEBUGCONTROL_INDEX) = newZwSystemDebugControl;
	
	oldZwCreateProcess = (ZWCREATEPROCESS)SYSTEMSERVICE(CREATEPROCESS_INDEX);
	(ZWCREATEPROCESS)SYSTEMSERVICE(CREATEPROCESS_INDEX) = newZwCreateProcess;
	
	oldZwCreateProcessEx = (ZWCREATEPROCESSEX)SYSTEMSERVICE(CREATEPROCESSEX_INDEX);
	(ZWCREATEPROCESSEX)SYSTEMSERVICE(CREATEPROCESSEX_INDEX) = newZwCreateProcessEx;
	
	oldZwMapViewOfSection = (ZWMAPVIEWOFSECTION)SYSTEMSERVICE(MAPVIEWOFSECTION_INDEX);
	(ZWMAPVIEWOFSECTION)SYSTEMSERVICE(MAPVIEWOFSECTION_INDEX) = newZwMapViewOfSection;
	
	oldZwDebugActiveProcess = (ZWDEBUGACTIVEPROCESS)SYSTEMSERVICE(DEBUGACTIVEPROCESS_INDEX);
	(ZWDEBUGACTIVEPROCESS)SYSTEMSERVICE(DEBUGACTIVEPROCESS_INDEX) = newZwDebugActiveProcess;
	
	oldZwOpenProcess = (ZWOPENPROCESS)SYSTEMSERVICE(OPENPROCESS_INDEX);
	(ZWOPENPROCESS)SYSTEMSERVICE(OPENPROCESS_INDEX) = newZwOpenProcess;
	
	oldZwOpenThread = (ZWOPENTHREAD)SYSTEMSERVICE(OPENTHREAD_INDEX);
	(ZWOPENTHREAD)SYSTEMSERVICE(OPENTHREAD_INDEX) = newZwOpenThread;
	
	oldZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)SYSTEMSERVICE(QUERYSYSTEMINFORMATION_INDEX);
	(ZWQUERYSYSTEMINFORMATION)SYSTEMSERVICE(QUERYSYSTEMINFORMATION_INDEX) = newZwQuerySystemInformation;
	
	oldZwCreateFile = (ZWCREATEFILE)SYSTEMSERVICE(CREATEFILE_INDEX);
	(ZWCREATEFILE)SYSTEMSERVICE(CREATEFILE_INDEX) = newZwCreateFile;
	
	oldZwReadFile = (ZWREADFILE)SYSTEMSERVICE(READFILE_INDEX);
	(ZWREADFILE)SYSTEMSERVICE(READFILE_INDEX) = newZwReadFile;
	
	oldZwWriteFile = (ZWWRITEFILE)SYSTEMSERVICE(WRITEFILE_INDEX);
	(ZWWRITEFILE)SYSTEMSERVICE(WRITEFILE_INDEX) = newZwWriteFile;
	
	oldZwDeleteFile = (ZWDELETEFILE)SYSTEMSERVICE(DELETEFILE_INDEX);
	(ZWDELETEFILE)SYSTEMSERVICE(DELETEFILE_INDEX) = newZwDeleteFile;
	
	oldZwSetInformationFile = (ZWSETINFORMATIONFILE)SYSTEMSERVICE(SETINFORMATIONFILE_INDEX);
	(ZWSETINFORMATIONFILE)SYSTEMSERVICE(SETINFORMATIONFILE_INDEX) = newZwSetInformationFile;
	
	oldZwQueryInformationFile = (ZWQUERYINFORMATIONFILE)SYSTEMSERVICE(QUERYINFORMATIONFILE_INDEX);
	(ZWQUERYINFORMATIONFILE)SYSTEMSERVICE(QUERYINFORMATIONFILE_INDEX) = newZwQueryInformationFile;

	oldZwCreateMutant = (ZWCREATEMUTANT)SYSTEMSERVICE(CREATEMUTANT_INDEX);
	(ZWCREATEMUTANT)SYSTEMSERVICE(CREATEMUTANT_INDEX) = newZwCreateMutant;
	
	enable_cr0();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs thread opening, and hides specific processes from the monitored processes.
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/bb432382(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/bb432382(v=vs.85).aspx
//	Process :
//		Calls the original function and if it succeeds, gets the targetThreadId by handle. If the targetProcessId is hidden
//		closes the handle and returns STATUS_INVALID_PARAMETER.
//		It the call failed, if ClientID is not NULL, copies the ClientID->UniqueThread parameter and
//		logs it. If ClientID is NULL (XP / s2003), copies the ObjectAttributes->ObjectName parameter
//		and logs it.
//	TODO :
//		- while blocking a call, restore the original *ThreadHandle value.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwOpenThread(PHANDLE ThreadHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientID)
{
	NTSTATUS statusCall, exceptionCode;
	ULONG currentProcessId, targetThreadId, targetProcessId;
	USHORT log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	PETHREAD eThread = NULL;
	
	ULONG kUniqueThread;
	HANDLE kThreadHandle;
	UNICODE_STRING kObjectName;

	kObjectName.Buffer = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWOPENTHREAD)(oldZwOpenThread))(ThreadHandle, DesiredAccess, ObjectAttributes, ClientID);
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		
		if(NT_SUCCESS(statusCall))
		{
			__try 
			{
				if(ExGetPreviousMode() != KernelMode)
					ProbeForRead(ThreadHandle, sizeof(HANDLE), 1);
				kThreadHandle = *ThreadHandle;
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				exceptionCode = GetExceptionCode();
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->ERROR",exceptionCode)))
					sendLogs(currentProcessId, L"ZwOpenThread", parameter);
				else
					sendLogs(currentProcessId, L"ZwOpenThread", L"0,-1,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->1");
				ExFreePool(parameter);
				return statusCall;
			}
		
			targetThreadId = getTIDByHandle(kThreadHandle);
			if(NT_SUCCESS(PsLookupThreadByThreadId((HANDLE)targetThreadId, &eThread)))
				targetProcessId = *(DWORD*)((PCHAR)eThread+0x1EC);
			else
				targetProcessId = 0;
			
			if(isProcessHiddenByPid(targetProcessId))
			{
				ZwClose(kThreadHandle);
				if(parameter && RtlStringCchPrintfW(parameter, MAXSIZE, L"0,3221225485,sss,ThreadHandle->ERROR,TID->%d,DesiredAccess->0x%08x", targetThreadId, DesiredAccess))
					sendLogs(currentProcessId, L"ZwOpenThread", parameter);
				else
					sendLogs(currentProcessId, L"ZwOpenThread", L"0,3221225485,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->ERROR");
				return STATUS_INVALID_PARAMETER;
			}
			
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,sss,ThreadHandle->0x%08x,TID->%d,DesiredAccess->0x%08x", kThreadHandle, targetThreadId, DesiredAccess)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(ClientID != NULL)
			{
				__try 
				{
					if(ExGetPreviousMode() != KernelMode)
						ProbeForRead(ClientID, sizeof(CLIENT_ID), 1);
					kUniqueThread = (ULONG)ClientID->UniqueThread;
				} 
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					exceptionCode = GetExceptionCode();
					if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->ERROR", exceptionCode)))
						sendLogs(currentProcessId, L"ZwOpenThread", parameter);
					else 
						sendLogs(currentProcessId, L"ZwOpenThread", L"0,-1,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->ERROR");
					if(parameter)
						ExFreePool(parameter);
					return statusCall;
				}
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,0,sss,ThreadHandle->ERROR,TID->%d,DesiredAccess->0x%08x", kUniqueThread, DesiredAccess)))
					log_lvl = LOG_PARAM;
			}
			else
			{
				__try 
				{
					if(ExGetPreviousMode() != KernelMode)
					{
						ProbeForRead(ObjectAttributes, sizeof(OBJECT_ATTRIBUTES), 1);
						ProbeForRead(ObjectAttributes->ObjectName, sizeof(UNICODE_STRING), 1);
						ProbeForRead(ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length, 1);
					}
					kObjectName.Length = ObjectAttributes->ObjectName->Length;
					kObjectName.MaximumLength = ObjectAttributes->ObjectName->Length;
					kObjectName.Buffer = ExAllocatePoolWithTag(NonPagedPool, kObjectName.MaximumLength, BUFFER_TAG);
					if(!kObjectName.Buffer)
					{
						if(parameter)
							ExFreePool(parameter);
						sendLogs(currentProcessId, L"ZwOpenThread", L"0,-1,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->ERROR");
						return statusCall;
					}
					RtlCopyUnicodeString(&kObjectName, ObjectAttributes->ObjectName);
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					exceptionCode = GetExceptionCode();
					if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->ERROR", exceptionCode)))
						sendLogs(currentProcessId, L"ZwOpenThread", parameter);
					else 
						sendLogs(currentProcessId, L"ZwOpenThread", L"0,-1,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->ERROR");
					if(parameter)
						ExFreePool(parameter);
					if(kObjectName.Buffer)
						ExFreePool(kObjectName.Buffer);
					return statusCall;
				}
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,0,sss,ThreadHandle->ERROR,TID->%wZ,DesiredAccess->0x%08x", &kObjectName, DesiredAccess)))
					log_lvl = LOG_PARAM;
				if(kObjectName.Buffer)
					ExFreePool(kObjectName.Buffer);
			}
		}

		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwOpenThread", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwOpenThread", L"0,-1,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwOpenThread", L"1,0,sss,ThreadHandle->ERROR,TID->ERROR,DesiredAccess->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	}

	return statusCall;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs process opening (mandatory for most of code injection techniques), and hides specific
//		processes from the monitored processes.
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567022(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567022(v=vs.85).aspx
//	Process :
//		Calls the original function and if it succeeds, gets the targetProcessId by handle. If the targetProcessId is hidden
//		closes the handle and returns STATUS_INVALID_PARAMETER.
//		It the call failed, if ClientID is not NULL, copies the ClientID->UniqueThread parameter and
//		logs it. If ClientID is NULL (XP / s2003), copies the ObjectAttributes->ObjectName parameter
//		and logs it.
//	TODO :
//		- while blocking a call, restore the original *ProcessHandle value.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwOpenProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientID)
{	
	NTSTATUS statusCall, status, exceptionCode;
	ULONG currentProcessId, i, targetProcessId;
	USHORT log_lvl = LOG_ERROR;
	UNICODE_STRING targetProcessName;
	PWCHAR parameter = NULL;
	
	HANDLE kRootDirectory, kProcessHandle;
	UNICODE_STRING kObjectName;
	ULONG kUniqueProcess;
	
	kObjectName.Buffer = NULL;
	targetProcessName.Buffer = NULL;

	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWOPENPROCESS)(oldZwOpenProcess))(ProcessHandle, DesiredAccess, ObjectAttributes, ClientID);
	if(isProcessMonitoredByPid(currentProcessId))
	{   	
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		targetProcessName.Length = 0;
		targetProcessName.MaximumLength = NTSTRSAFE_UNICODE_STRING_MAX_CCH * sizeof(WCHAR);
		targetProcessName.Buffer = ExAllocatePoolWithTag(NonPagedPool, targetProcessName.MaximumLength, PROCNAME_TAG);
		
		if(NT_SUCCESS(statusCall))
		{
			__try 
			{
				if(ExGetPreviousMode() != KernelMode)
					ProbeForRead(ProcessHandle, sizeof(HANDLE), 1);
				kProcessHandle = *ProcessHandle;
			} 
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				exceptionCode = GetExceptionCode();
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,ProcessHandle->ERROR,ProcessName->ERROR,PID->ERROR,DesiredAccess->ERROR",exceptionCode)))
					sendLogs(currentProcessId, L"ZwOpenProcess", parameter);
				else
					sendLogs(currentProcessId, L"ZwOpenProcess", L"0,-1,ssss,ProcessHandle->ERROR,ProcessName->ERROR,PID->ERROR,DesiredAccess->1");
				ExFreePool(parameter);
				return statusCall;;
			}
		
			targetProcessId = getPIDByHandle(kProcessHandle);
			if(targetProcessName.Buffer)
				status = getProcNameByPID(targetProcessId, &targetProcessName);
			else
				status = STATUS_NOT_FOUND;
				
			if(isProcessHiddenByPid(targetProcessId))
			{
				ZwClose(kProcessHandle);
				if(parameter && NT_SUCCESS(status) && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,-1,ssss,ProcessHandle->0x%08x,ProcessName->%wZ,PID->%d,DesiredAccess->0x%08x", kProcessHandle, &targetProcessName, targetProcessId, DesiredAccess)))
					sendLogs(currentProcessId, L"ZwOpenProcess", parameter);
				else
					sendLogs(currentProcessId, L"ZwOpenProcess", L"0,-1,ssss,ProcessHandle->ERROR,ProcessName->ERROR_HIDDEN,PID->ERROR,DesiredAccess->ERROR");
				
				if(targetProcessName.Buffer)
					ExFreePool(targetProcessName.Buffer);
				if(parameter)
					ExFreePool(parameter);
				return STATUS_INVALID_PARAMETER;
			}
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(status) && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,ssss,ProcessHandle->0x%08x,ProcessName->%wZ,PID->%d,DesiredAccess->0x%08x", kProcessHandle, &targetProcessName, targetProcessId, DesiredAccess)))
				log_lvl = LOG_PARAM;
		}
		else
		{	
			log_lvl = LOG_ERROR;
			if(ClientID != NULL)
			{
				__try 
				{
					if(ExGetPreviousMode() != KernelMode)
						ProbeForRead(ClientID, sizeof(CLIENT_ID), 1);
					kUniqueProcess = (ULONG)ClientID->UniqueProcess;
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					exceptionCode = GetExceptionCode();
					if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,ProcessHandle->ERROR,ProcessName->ERROR,PID->ERROR,DesiredAccess->ERROR", exceptionCode)))
						sendLogs(currentProcessId, L"ZwOpenProcess", parameter);
					else 
						sendLogs(currentProcessId, L"ZwOpenProcess", L"0,-1,ssss,ProcessHandle->ERROR,ProcessName->ERROR,PID->ERROR,DesiredAccess->ERROR");
					if(parameter)
						ExFreePool(parameter);
					if(targetProcessName.Buffer)
						ExFreePool(targetProcessName.Buffer);
					return statusCall;
				}
				
				if(targetProcessName.Buffer)
					status = getProcNameByPID(kUniqueProcess, &targetProcessName);
				else
					status = STATUS_NOT_FOUND;
				
				if(parameter && NT_SUCCESS(status) && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,ProcessHandle->ERROR,ProcessName->%wZ,PID->%d,DesiredAccess->0x%08x", statusCall,&targetProcessName, kUniqueProcess, DesiredAccess)))
					log_lvl = LOG_PARAM;
			}
			else
			{
				__try 
				{
					if(ExGetPreviousMode() != KernelMode)
					{
						ProbeForRead(ObjectAttributes, sizeof(OBJECT_ATTRIBUTES), 1);
						ProbeForRead(ObjectAttributes->ObjectName, sizeof(UNICODE_STRING), 1);
						ProbeForRead(ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length, 1);
					}
					kObjectName.Length = ObjectAttributes->ObjectName->Length;
					kObjectName.MaximumLength = ObjectAttributes->ObjectName->Length;
					kObjectName.Buffer = ExAllocatePoolWithTag(NonPagedPool, kObjectName.MaximumLength, BUFFER_TAG);
					if(kObjectName.Buffer)
						RtlCopyUnicodeString(&kObjectName, ObjectAttributes->ObjectName);
					else
					{
						if(parameter)
							ExFreePool(parameter);
						if(targetProcessName.Buffer)
							ExFreePool(targetProcessName.Buffer);
						sendLogs(currentProcessId, L"ZwOpenProcess", L"0,-1,ssss,ProcessHandle->ERROR,ProcessName->ERROR,PID->ERROR,DesiredAccess->ERROR");
						return statusCall;
					}
				}
				__except (EXCEPTION_EXECUTE_HANDLER)
				{
					exceptionCode = GetExceptionCode();
					if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,ProcessHandle->ERROR,ProcessName->ERROR,PID->ERROR,DesiredAccess->ERROR", exceptionCode)))
						sendLogs(currentProcessId, L"ZwOpenProcess", parameter);
					else 
						sendLogs(currentProcessId, L"ZwOpenProcess", L"0,-1,ssss,ProcessHandle->ERROR,ProcessName->ERROR,PID->ERROR,DesiredAccess->ERROR");
					if(parameter)
						ExFreePool(parameter);
					if(targetProcessName.Buffer)
						ExFreePool(targetProcessName.Buffer);
					if(kObjectName.Buffer)
						ExFreePool(kObjectName.Buffer);
					return statusCall;
				}
				if(parameter && kObjectName.Buffer && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,ProcessHandle->ERROR,ProcessName->%wZ,PID->ERROR,DesiredAccess->0x%08x", statusCall,&kObjectName,DesiredAccess)))
				{
					log_lvl = LOG_PARAM;
					ExFreePool(kObjectName.Buffer);
				}
			}
		}		
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwOpenProcess", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwOpenProcess", L"0,-1,ssss,ProcessHandle->ERROR,ProcessName->ERROR,PID->ERROR,DesiredAccess->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwOpenProcess", L"1,0,ssss,ProcessHandle->ERROR,ProcessName->ERROR,PID->ERROR,DesiredAccess->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
		if(targetProcessName.Buffer)
			ExFreePool(targetProcessName.Buffer);
	}
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs process information retrieval, and hides specific processes from the monitored processes.
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/windows/desktop/ms725506(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/windows/desktop/ms725506(v=vs.85).aspx
//	Process :
//		Checks the information type. If SystemProcessInformation (enumerate running processes), the
//		hidden targetProcessIds are unlinked from the result (SYSTEM_PROCESS_INFORMATION linked list).
//	Todo :
//		- Hide also thread listing
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength)
{
	NTSTATUS statusCall;
	ULONG currentProcessId, targetThreadId, i;
	USHORT log_lvl = LOG_ERROR;
	PSYSTEM_PROCESS_INFORMATION pSystemProcessInformation = NULL, pPrev = NULL;
	PWCHAR parameter = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	
	statusCall = ((ZWQUERYSYSTEMINFORMATION)(oldZwQuerySystemInformation))(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	if(isProcessMonitoredByPid(currentProcessId))
	{
		if(NT_SUCCESS(statusCall))
		{
			if(SystemInformationClass == SystemProcessInformation)
			{
				pSystemProcessInformation = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
				pPrev = pSystemProcessInformation;
				
				while(pSystemProcessInformation->NextEntryOffset)
				{
					if(isProcessHiddenByPid((ULONG)pSystemProcessInformation->ProcessId))
						pPrev->NextEntryOffset += pSystemProcessInformation->NextEntryOffset;	// UNLINK
					
					pPrev = pSystemProcessInformation;
					pSystemProcessInformation = (PSYSTEM_PROCESS_INFORMATION)((char*)pSystemProcessInformation + pSystemProcessInformation->NextEntryOffset);
				}
				
				sendLogs(currentProcessId, L"ZwQuerySystemInformation",L"1,0,s,SystemInformationClass->5");
				return statusCall;
			}
			else
			{
				log_lvl = LOG_SUCCESS;
				parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,s,SystemInformationClass->%d", SystemInformationClass)))
					log_lvl = LOG_PARAM;
			}
		}
		else
		{
			log_lvl = LOG_ERROR;
			parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,s,SystemInformationClass->%d", statusCall, SystemInformationClass)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwQuerySystemInformation", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwQuerySystemInformation", L"0,-1,s,SystemInformationClass->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwQuerySystemInformation", L"1,0,s,SystemInformationClass->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	}
	
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs virtual memory modification.
//	Parameters :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/Memory%20Management/Virtual%20Memory/NtWriteVirtualMemory.html
//	Return value :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/Memory%20Management/Virtual%20Memory/NtWriteVirtualMemory.html
//	Process :
//		Adds the process to the monitored processes list and logs the BaseAddress, Buffer, ProcessHandle, and NumberOfBytesToWrite parameters.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwWriteVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten)
{
	NTSTATUS statusCall;
	ULONG currentProcessId, targetProcessId;
	ULONG log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWWRITEVIRTUALMEMORY)(oldZwWriteVirtualMemory))(ProcessHandle, BaseAddress, Buffer, NumberOfBytesToWrite, NumberOfBytesWritten);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		targetProcessId = getPIDByHandle(ProcessHandle);
		
		if(NT_SUCCESS(statusCall) && targetProcessId)
			startMonitoringProcess(targetProcessId);
		
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,sssss,ProcessHandle->0x%08x,PID->%d,BaseAddress->0x%08x,Buffer->0x%08x,NumberOfBytesToWrite->%d", ProcessHandle, targetProcessId, BaseAddress, Buffer, NumberOfBytesToWrite)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sssss,ProcessHandle->0x%08x,PID->%d,BaseAddress->0x%08x,Buffer->0x%08x,NumberOfBytesToWrite->%d", statusCall, ProcessHandle, targetProcessId, BaseAddress, Buffer, NumberOfBytesToWrite)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwWriteVirtualMemory", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwWriteVirtualMemory", L"0,1,sssss,ProcessHandle->ERROR,PID->ERROR,BaseAddress->ERROR,Buffer->ERROR,NumberOfBytesToWrite->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwWriteVirtualMemory", L"1,0,sssss,ProcessHandle->ERROR,PID->ERROR,BaseAddress->ERROR,Buffer->ERROR,NumberOfBytesToWrite->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	
	}
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs process debugging (may be used for code injection).
//	Parameters :
//		See http://www.openrce.org/articles/full_view/26
//	Return value :
//		See http://www.openrce.org/articles/full_view/26
//	Process :
//		Adds the process to the monitored processes list and logs the targetProcessId, ProcessHandle and DebugHandle parameters
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwDebugActiveProcess(HANDLE ProcessHandle, HANDLE DebugHandle)
{
	NTSTATUS statusCall;
	ULONG targetProcessId, currentProcessId;
	USHORT log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWDEBUGACTIVEPROCESS)(oldZwDebugActiveProcess))(ProcessHandle, DebugHandle);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{		
		targetProcessId = getPIDByHandle(ProcessHandle);
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,sss,ProcessHandle->0x%08x,PID->%d,DebugHandle->0x%08x", ProcessHandle, targetProcessId, DebugHandle)))
				log_lvl = LOG_PARAM;
			if(targetProcessId)
				startMonitoringProcess(targetProcessId);
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sss,ProcessHandle->0x%08x,PID->%d,DebugHandle->0x%08x", statusCall, ProcessHandle, targetProcessId, DebugHandle)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwDebugActiveProcess", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwDebugActiveProcess", L"0,-1,sss,ProcessHandle->ERROR,PID->ERROR,DebugHandle->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwDebugActiveProcess", L"1,0,sss,ProcessHandle->ERROR,PID->ERROR,DebugHandle->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	}
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs process creation.
//	Parameters :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/NT%20Objects/Process/NtCreateProcess.html
//	Return value :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/NT%20Objects/Process/NtCreateProcess.html
//	Process :
//		Starts the process, gets its targetProcessId and adds it to the monitored processes list, copies
//		ObjectAttributes->ObjectName parameter then logs.
//	TODO : 
//		- also log ProcessHandle, DesiredAccess, InheritObjectTable, ParentProcess
//		- also log Filename (& Commandline) parameters
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwCreateProcess(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE ParentProcess, BOOLEAN InheritObjectTable, HANDLE SectionHandle, HANDLE DebugPort, HANDLE ExceptionPort)
{
	NTSTATUS statusCall, exceptionCode;
	ULONG currentProcessId, childProcessId;
	USHORT log_lvl = LOG_ERROR;
	UNICODE_STRING full_path;
	POBJECT_NAME_INFORMATION nameInformation = NULL;
	PWCHAR parameter = NULL;

	HANDLE kRootDirectory, kProcessHandle;
	UNICODE_STRING kObjectName;
	
	full_path.Buffer = NULL;
	kObjectName.Buffer = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWCREATEPROCESS)(oldZwCreateProcess))(ProcessHandle, DesiredAccess, ObjectAttributes, ParentProcess, InheritObjectTable, SectionHandle, DebugPort, ExceptionPort);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		kObjectName.Buffer = NULL;
		
		__try 
		{
			if(ExGetPreviousMode() != KernelMode)
			{
				ProbeForRead(ProcessHandle, sizeof(HANDLE), 1);
				if(ObjectAttributes)
				{
					ProbeForRead(ObjectAttributes, sizeof(OBJECT_ATTRIBUTES), 1);
					ProbeForRead(ObjectAttributes->ObjectName, sizeof(UNICODE_STRING), 1);
					ProbeForRead(ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length, 1);
				}
			}	
			kProcessHandle = *ProcessHandle;
			childProcessId = getPIDByHandle(kProcessHandle);
			if(ObjectAttributes)
			{
				kRootDirectory = ObjectAttributes->RootDirectory;
				kObjectName.Length = ObjectAttributes->ObjectName->Length;
				kObjectName.MaximumLength = ObjectAttributes->ObjectName->MaximumLength;
				kObjectName.Buffer = ExAllocatePoolWithTag(NonPagedPool, kObjectName.MaximumLength, BUFFER_TAG);
				RtlCopyUnicodeString(&kObjectName, ObjectAttributes->ObjectName);
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			exceptionCode = GetExceptionCode();
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssssss,ProcessHandle->ERROR,PID->ERROR,DesiredAccess->ERROR,InheritObjectTable->ERROR,ParentProcess->ERROR,FileName->ERROR", exceptionCode)))
				sendLogs(currentProcessId, L"ZwCreateProcess", parameter);
			else 
				sendLogs(currentProcessId, L"ZwCreateProcess", L"0,-1,ssssss,ProcessHandle->ERROR,PID->ERROR,DesiredAccess->ERROR,InheritObjectTable->ERROR,ParentProcess->ERROR,FileName->ERROR");
			ExFreePool(parameter);
			if(kObjectName.Buffer)
				ExFreePool(kObjectName.Buffer);
			return statusCall;;
		}
		
		if(kRootDirectory && ObjectAttributes)	// handle the not null rootdirectory case
		{
			// allocate both name information struct and unicode string buffer
			nameInformation = ExAllocatePoolWithTag(NonPagedPool, MAXSIZE, BUFFER_TAG);
			if(nameInformation)
			{
				if(NT_SUCCESS(ZwQueryObject(kRootDirectory, ObjectNameInformation, nameInformation, MAXSIZE, NULL)))
				{
					full_path.MaximumLength = nameInformation->Name.Length + kObjectName.Length + 2 + sizeof(WCHAR);
					full_path.Buffer = ExAllocatePoolWithTag(NonPagedPool, full_path.MaximumLength, BUFFER_TAG);
					RtlZeroMemory(full_path.Buffer, full_path.MaximumLength);
					RtlCopyUnicodeString(&full_path, &(nameInformation->Name));
					RtlAppendUnicodeToString(&full_path, L"\\");
					RtlAppendUnicodeStringToString(&full_path, &kObjectName);
				}
			}
		}
		else
			RtlInitUnicodeString(&full_path, kObjectName.Buffer);
		
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,ssssss,ProcessHandle->0x%08x,PID->%d,DesiredAccess->0x%08x,InheritObjectTable->%d,ParentProcess->0x%08x,FileName->%wZ", kProcessHandle,childProcessId,DesiredAccess,InheritObjectTable,ParentProcess,&full_path)))
				log_lvl = LOG_PARAM;
			if(childProcessId)
				startMonitoringProcess(childProcessId);
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssssss,ProcessHandle->0x%08x,PID->%d,DesiredAccess->0x%08x,InheritObjectTable->%d,ParentProcess->0x%08x,FileName->%wZ", statusCall,kProcessHandle,childProcessId,DesiredAccess,InheritObjectTable,ParentProcess,&full_path)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwCreateProcess", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwCreateProcess", L"0,-1,ssssss,ProcessHandle->ERROR,PID->ERROR,DesiredAccess->ERROR,InheritObjectTable->ERROR,ParentProcess->ERROR,FileName->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwCreateProcess", L"1,0,ssssss,ProcessHandle->ERROR,PID->ERROR,DesiredAccess->ERROR,InheritObjectTable->ERROR,ParentProcess->ERROR,FileName->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	}
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs process creation.
//	Parameters :
//		See http://www.tech-archive.net/Archive/Development/microsoft.public.win32.programmer.kernel/2004-02/0195.html (lulz)
//	Return value :
//		See http://www.tech-archive.net/Archive/Development/microsoft.public.win32.programmer.kernel/2004-02/0195.html (lulz)
//	Process :
//		Starts the process, gets its targetProcessId and adds it to the monitored processes list, copies
//		ObjectAttributes->ObjectName parameter then logs.
////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwCreateProcessEx(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE InheritFromProcessHandle, BOOLEAN InheritHandles, HANDLE SectionHandle, HANDLE DebugPort, HANDLE ExceptionPort, HANDLE dunno)
{
	NTSTATUS statusCall, exceptionCode;
	ULONG currentProcessId, childProcessId;
	USHORT log_lvl = LOG_ERROR;
	UNICODE_STRING full_path;
	PWCHAR parameter = NULL;
	POBJECT_NAME_INFORMATION nameInformation = NULL;
	
	HANDLE kRootDirectory, kProcessHandle;
	UNICODE_STRING kObjectName;
	
	full_path.Buffer = NULL;
	kObjectName.Buffer = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWCREATEPROCESSEX)(oldZwCreateProcessEx))(ProcessHandle, DesiredAccess, ObjectAttributes, InheritFromProcessHandle, InheritHandles, SectionHandle, DebugPort, ExceptionPort, dunno);	 
		
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		kObjectName.Buffer = NULL;
		
		__try 
		{
			if(ExGetPreviousMode() != KernelMode)
			{
				ProbeForRead(ProcessHandle, sizeof(HANDLE), 1);
				if(ObjectAttributes)
				{
					ProbeForRead(ObjectAttributes, sizeof(OBJECT_ATTRIBUTES), 1);
					ProbeForRead(ObjectAttributes->ObjectName, sizeof(UNICODE_STRING), 1);
					ProbeForRead(ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length, 1);
				}
			}
			kProcessHandle = *ProcessHandle;
			childProcessId = getPIDByHandle(kProcessHandle);
			if(ObjectAttributes)
			{
				kRootDirectory = ObjectAttributes->RootDirectory;
				kObjectName.Length = ObjectAttributes->ObjectName->Length;
				kObjectName.MaximumLength = ObjectAttributes->ObjectName->MaximumLength;
				kObjectName.Buffer = ExAllocatePoolWithTag(NonPagedPool, kObjectName.MaximumLength, BUFFER_TAG);
				RtlCopyUnicodeString(&kObjectName, ObjectAttributes->ObjectName);
			}
		} 
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			exceptionCode = GetExceptionCode();
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssssss,ProcessHandle->ERROR,PID->ERROR,DesiredAccess->ERROR,InheritHandles->ERROR,InheritFromProcessHandle->ERROR,FileName->ERROR", exceptionCode)))
				sendLogs(currentProcessId, L"ZwCreateProcessEx", parameter);
			else 
				sendLogs(currentProcessId, L"ZwCreateProcessEx", L"0,-1,ssssss,ProcessHandle->ERROR,PID->ERROR,DesiredAccess->ERROR,InheritHandles->ERROR,InheritFromProcessHandle->ERROR,FileName->ERROR");
			ExFreePool(parameter);
			if(kObjectName.Buffer)
				ExFreePool(kObjectName.Buffer);
			return statusCall;
		}
		
		if(kRootDirectory && ObjectAttributes)	// handle the not null rootdirectory case
		{
			// allocate both name information struct and unicode string buffer
			nameInformation = ExAllocatePoolWithTag(NonPagedPool, MAXSIZE, BUFFER_TAG);
			if(nameInformation)
			{
				if(NT_SUCCESS(ZwQueryObject(kRootDirectory, ObjectNameInformation, nameInformation, MAXSIZE, NULL)))
				{
					full_path.MaximumLength = nameInformation->Name.Length + kObjectName.Length + 2 + sizeof(WCHAR);
					full_path.Buffer = ExAllocatePoolWithTag(NonPagedPool, full_path.MaximumLength, BUFFER_TAG);
					RtlZeroMemory(full_path.Buffer, full_path.MaximumLength);
					RtlCopyUnicodeString(&full_path, &(nameInformation->Name));
					RtlAppendUnicodeToString(&full_path, L"\\");
					RtlAppendUnicodeStringToString(&full_path, &kObjectName);
				}
				else
					RtlInitUnicodeString(&full_path, kObjectName.Buffer);
			}
			else
				RtlInitUnicodeString(&full_path, kObjectName.Buffer);
		}
		else
			RtlInitUnicodeString(&full_path, kObjectName.Buffer);
		
		
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,ssssss,ProcessHandle->0x%08x,PID->%d,DesiredAccess->0x%08x,InheritHandles->%d,InheritFromProcessHandle->0x%08x,FileName->%wZ", kProcessHandle,childProcessId,DesiredAccess,InheritHandles,InheritFromProcessHandle,&full_path)))
				log_lvl = LOG_PARAM;
			
			if(childProcessId)
				startMonitoringProcess(childProcessId);
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssssss,ProcessHandle->0x%08x,PID->%d,DesiredAccess->0x%08x,InheritObjectTable->%d,ParentProcess->0x%08x,FileName->%wZ", statusCall,kProcessHandle,childProcessId,DesiredAccess,InheritHandles,InheritFromProcessHandle,&full_path)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwCreateProcessEx", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwCreateProcessEx", L"0,-1,ssssss,ProcessHandle->ERROR,PID->ERROR,DesiredAccess->ERROR,InheritHandles->ERROR,InheritFromProcessHandle->ERROR,FileName->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwCreateProcessEx", L"1,0,ssssss,ProcessHandle->ERROR,PID->ERROR,DesiredAccess->ERROR,InheritHandles->ERROR,InheritFromProcessHandle->ERROR,FileName->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	}
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs thread-based Asynchronous Procedure Call creation (may be used for code injection).
//	Parameters :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/APC/NtQueueApcThread.html
//	Return value :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/APC/NtQueueApcThread.html
//	Process :
//		Proceed the call then gets the thread owner and adds it to the monitored processes list, then
//		log.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwQueueApcThread(HANDLE ThreadHandle, PIO_APC_ROUTINE ApcRoutine, PVOID ApcRoutineContext, PIO_STATUS_BLOCK ApcStatusBlock, ULONG ApcReserved)
{
	NTSTATUS statusCall;
	ULONG currentProcessId, targetThreadId;
	DWORD targetProcessId;
	USHORT log_lvl = LOG_ERROR;
	PETHREAD eThread = NULL;
	PWCHAR parameter = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWQUEUEAPCTHREAD)(oldZwQueueApcThread))(ThreadHandle, ApcRoutine, ApcRoutineContext, ApcStatusBlock, ApcReserved);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		targetThreadId = getTIDByHandle(ThreadHandle);
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		
		if(NT_SUCCESS(PsLookupThreadByThreadId((HANDLE)targetThreadId, &eThread)))
			targetProcessId = *(DWORD*)((PCHAR)eThread+0x1EC);
		else
			targetProcessId = 0;
			
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,ssss,ThreadHandle->0x%08x,TID->%d,PID->%d,ApcRoutine->0x%08x", ThreadHandle, targetThreadId, targetProcessId, ApcRoutine)))
				log_lvl = LOG_PARAM;
			
			if(targetProcessId)
				startMonitoringProcess(targetProcessId);
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,ThreadHandle->0x%08x,TID->%d,PID->%d,ApcRoutine->0x%08x", statusCall, ThreadHandle, targetThreadId, targetProcessId, ApcRoutine)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwQueueApcThread", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwQueueApcThread", L"0,-1,ssss,ThreadHandle->ERROR,TID->ERROR,PID->ERROR,ApcRoutine->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwQueueApcThread", L"1,0,ssss,ThreadHandle->ERROR,TID->ERROR,PID->ERROR,ApcRoutine->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	}

	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs thread creation.
//	Parameters :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/NT%20Objects/Thread/NtCreateThread.html
//	Return value :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/NT%20Objects/Thread/NtCreateThread.html
//	Process :
//		Gets the thread's owner, proceeds the call then adds immediately the targetProcessId to the monitored
//		processes list if it succeeded. Then logs.
//	Notes :
//		Actually, a race condition occurs : we must create the thread before adding the remote process
//		to the list in order to check if the thread was created successfuly. However, a malware would
//		not create a thread without giving it nothing to execute, right? :]
//	TODO :
//		- Create the thread in suspended state and resume it after adding the process to the list to avoid
//		race condition issues.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwCreateThread(PHANDLE ThreadHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, HANDLE ProcessHandle, PCLIENT_ID ClientID, PCONTEXT ThreadContext, PINITIAL_TEB InitialTeb, BOOLEAN CreateSuspended)
{
	NTSTATUS statusCall, exceptionCode;
	ULONG currentProcessId, targetProcessId, createdThreadId;
	USHORT log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	
	HANDLE kThreadHandle;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	
	targetProcessId = getPIDByHandle(ProcessHandle);	// faster than placing it after the monitored process check
	statusCall = ((ZWCREATETHREAD)(oldZwCreateThread))(ThreadHandle, DesiredAccess, ObjectAttributes, ProcessHandle, ClientID, ThreadContext, InitialTeb, CreateSuspended);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		if(NT_SUCCESS(statusCall) && targetProcessId)
			startMonitoringProcess(targetProcessId);	// <-- RACE CONDITION
		
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		_try
		{
			if(ExGetPreviousMode() != KernelMode)
				ProbeForRead(ThreadHandle, sizeof(HANDLE), 1);
			kThreadHandle = *ThreadHandle;
		} 
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			exceptionCode = GetExceptionCode();
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sssss,PID->ERROR,ThreadHandle->ERROR,TID->ERROR,CreateSuspended->ERROR,DesiredAccess->ERROR", exceptionCode)))
				sendLogs(currentProcessId, L"ZwCreateThread", parameter);
			else 
				sendLogs(currentProcessId, L"ZwCreateThread", L"0,-1,sssss,PID->ERROR,ThreadHandle->ERROR,TID->ERROR,CreateSuspended->ERROR,DesiredAccess->ERROR");
			if(parameter)
				ExFreePool(parameter);
			return statusCall;
		}
		
		createdThreadId = getTIDByHandle(kThreadHandle);
		
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,sssss,PID->%d,ThreadHandle->0x%08x,TID->%d,CreateSuspended->%d,DesiredAccess->0x%08x", kThreadHandle, targetProcessId, createdThreadId, CreateSuspended, DesiredAccess)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sssss,PID->%d,ThreadHandle->0x%08x,TID->%d,CreateSuspended->%d,DesiredAccess->0x%08x", statusCall, kThreadHandle, targetProcessId, createdThreadId, CreateSuspended, DesiredAccess)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwCreateThread", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwCreateThread", L"0,-1,sssss,PID->ERROR,ThreadHandle->ERROR,TID->ERROR,CreateSuspended->ERROR,DesiredAccess->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwCreateThread", L"1,0,sssss,PID->ERROR,ThreadHandle->ERROR,TID->ERROR,CreateSuspended->ERROR,DesiredAccess->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	}
	
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs section mapping (may be used for code injection).
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff566481(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff566481(v=vs.85).aspx
//	Process :
//		Proceeds the call, then if the process is not the current one, adds it to the monitored
//		processes list then logs it.
//	TODO :
//		- Also log SectionOffset, ViewSize
//		- Check if a race condition occurs (the process is not added before the call is passed)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwMapViewOfSection(HANDLE SectionHandle, HANDLE ProcessHandle, PVOID *BaseAddress, ULONG_PTR ZeroBits, SIZE_T CommitSize, PLARGE_INTEGER SectionOffset, PSIZE_T ViewSize, SECTION_INHERIT InheritDisposition, ULONG AllocationType, ULONG Win32Protect)
{
	NTSTATUS statusCall;
	ULONG targetProcessId, currentProcessId;
	USHORT log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWMAPVIEWOFSECTION)(oldZwMapViewOfSection))(SectionHandle, ProcessHandle, BaseAddress, ZeroBits, CommitSize, SectionOffset, ViewSize, InheritDisposition, AllocationType, Win32Protect);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		targetProcessId = getPIDByHandle(ProcessHandle);
		
		if(currentProcessId != targetProcessId)
		{
			parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
			
			if(NT_SUCCESS(statusCall))
			{
				log_lvl = LOG_SUCCESS;
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,sssss,ProcessHandle->0x%08x,PID->%d,BaseAddress->0x%08x,SectionHandle->0x%08x,Win32Protect->%d", ProcessHandle, targetProcessId, BaseAddress, SectionHandle, Win32Protect)))
					log_lvl = LOG_PARAM;
				if(targetProcessId)
					startMonitoringProcess(targetProcessId);
			}
			else
			{
				log_lvl = LOG_ERROR;
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sssss,ProcessHandle->0x%08x,PID->%d,BaseAddress->0x%08x,SectionHandle->0x%08x,Win32Protect->%d", statusCall, ProcessHandle, targetProcessId, BaseAddress, SectionHandle, Win32Protect)))
					log_lvl = LOG_PARAM;
			}
			
			switch(log_lvl)
			{
				case LOG_PARAM:
					sendLogs(currentProcessId, L"NtMapViewOfSection", parameter);
				break;
				case LOG_SUCCESS:
					sendLogs(currentProcessId, L"NtMapViewOfSection", L"0,-1,sssss,ProcessHandle->ERROR,PID->ERROR,BaseAddress->ERROR,SectionHandle->ERROR,Win32Protect->ERROR");
				break;
				default:
					sendLogs(currentProcessId, L"NtMapViewOfSection", L"1,0,sssss,ProcessHandle->ERROR,PID->ERROR,BaseAddress->ERROR,SectionHandle->ERROR,Win32Protect->ERROR");
				break;
			}
			if(parameter != NULL)
				ExFreePool(parameter);
		}
	}
	
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs thread context manipulation (may be used for code injection).
//	Parameters :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/NT%20Objects/Thread/Thread%20Context/NtSetContextThread.html
//	Return value :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/NT%20Objects/Thread/Thread%20Context/NtSetContextThread.html
//	Process :
//		Pass the call, adds the process (thread owner) to the monitored processes list and logs.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwSetContextThread(HANDLE ThreadHandle, PCONTEXT Context)
{
	NTSTATUS statusCall;
	ULONG currentProcessId, targetThreadId;
	DWORD targetProcessId;
	USHORT log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	PETHREAD eThread = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWSETCONTEXTTHREAD)(oldZwSetContextThread))(ThreadHandle, Context);
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		
		targetThreadId = getTIDByHandle(ThreadHandle);
		
		if(NT_SUCCESS(PsLookupThreadByThreadId((HANDLE)targetThreadId, &eThread)))
			targetProcessId = *(DWORD*)((PCHAR)eThread+0x1EC);
		else
			targetProcessId = -1;
			
			
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,sss,ThreadHandle->0x%08x,TID->%d,PID->%d", ThreadHandle, targetThreadId, targetProcessId)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,%d,sss,ThreadHandle->0x%08x,TID->%d,PID->%d", statusCall, ThreadHandle, targetThreadId, targetProcessId)))
				log_lvl = LOG_PARAM;
		}
	
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwSetContextThread", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwSetContextThread", L"0,-1,sss,ThreadHandle->ERROR,TID->ERROR,PID->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwSetContextThread", L"1,0,sss,ThreadHandle->ERROR,TID->ERROR,PID->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	}

	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs process debugging operations (may be used for code injection).
//	Parameters :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/Debug/NtSystemDebugControl.html
//	Return value :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/Debug/NtSystemDebugControl.html
//	Process :
//		Pass the call, adds the process to the monitored processes list and logs.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwSystemDebugControl(SYSDBG_COMMAND Command, PVOID InputBuffer, ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLength, PULONG ReturnLength)
{
	NTSTATUS statusCall;
	ULONG currentProcessId;
	USHORT log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWSYSTEMDEBUGCONTROL)(oldZwSystemDebugControl))(Command, InputBuffer, InputBufferLength, OutputBuffer, OutputBufferLength, ReturnLength);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,s,Command->%d", Command)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,%d,s,Command->%d", statusCall, Command)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwSystemDebugControl", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwSystemDebugControl", L"0,-1,s,Command->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwSystemDebugControl", L"1,0,s,Command->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
	}
	
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs file creation and/or file opening.
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff566424(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff566424(v=vs.85).aspx
//	Process :
//		Copies arguments, handles the non-NULL ObjectAttributes->RootDirectory parameter case (concat.
//		of RootDirectory and ObjectName) then log.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwCreateFile(PHANDLE FileHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PIO_STATUS_BLOCK IoStatusBlock, PLARGE_INTEGER AllocationSize, ULONG FileAttributes, ULONG ShareAccess, ULONG CreateDisposition, ULONG CreateOptions, PVOID EaBuffer, ULONG EaLength)
{
	NTSTATUS statusCall, exceptionCode;
	ULONG currentProcessId, returnLength;
	USHORT log_lvl = LOG_ERROR;
	UNICODE_STRING full_path;
	PWCHAR parameter = NULL;
	POBJECT_NAME_INFORMATION nameInformation = NULL;
	
	HANDLE kRootDirectory, kFileHandle;
	UNICODE_STRING kObjectName;
	
	full_path.Buffer = NULL;
	kObjectName.Buffer = NULL;
	
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWCREATEFILE)(oldZwCreateFile))(FileHandle, DesiredAccess, ObjectAttributes, IoStatusBlock, AllocationSize, FileAttributes, ShareAccess, CreateDisposition, CreateOptions, EaBuffer, EaLength);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		kObjectName.Buffer = NULL;
		
		__try
		{
			if(ExGetPreviousMode() != KernelMode)
			{
				ProbeForRead(FileHandle, sizeof(HANDLE), 1);
				ProbeForRead(ObjectAttributes, sizeof(OBJECT_ATTRIBUTES), 1);
				ProbeForRead(ObjectAttributes->ObjectName, sizeof(UNICODE_STRING), 1);
				ProbeForRead(ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length, 1);
			}
			kFileHandle = *FileHandle;
			kRootDirectory = ObjectAttributes->RootDirectory;
			kObjectName.Length = ObjectAttributes->ObjectName->Length;
			kObjectName.MaximumLength = ObjectAttributes->ObjectName->MaximumLength;
			kObjectName.Buffer = ExAllocatePoolWithTag(NonPagedPool, kObjectName.MaximumLength, BUFFER_TAG);
			RtlCopyUnicodeString(&kObjectName, ObjectAttributes->ObjectName);
			
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			exceptionCode = GetExceptionCode();
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sssssss,FileHandle->ERROR,FileName->ERROR,DesiredAccess->ERROR,CreateDisposition->ERROR,CreateOptions->ERROR,FileAttributes->ERROR,ShareAccess->ERROR", exceptionCode)))
				sendLogs(currentProcessId, L"ZwCreateFile", parameter);
			else 
				sendLogs(currentProcessId ,L"ZwCreateFile", L"0,-1,sssssss,FileHandle->ERROR,FileName->ERROR,DesiredAccess->ERROR,CreateDisposition->ERROR,CreateOptions->ERROR,FileAttributes->ERROR,ShareAccess->ERROR");
			ExFreePool(parameter);
			if(kObjectName.Buffer)
				ExFreePool(kObjectName.Buffer);
			return statusCall;
		}
		
		if(kRootDirectory)	// handle the not null rootdirectory case
		{
			// allocate both name information struct and unicode string buffer
			nameInformation = ExAllocatePoolWithTag(NonPagedPool, MAXSIZE, BUFFER_TAG);
			if(nameInformation)
			{
				if(NT_SUCCESS(ZwQueryObject(kRootDirectory, ObjectNameInformation, nameInformation, MAXSIZE, NULL)))
				{
					full_path.MaximumLength = nameInformation->Name.Length + kObjectName.Length + 2 + sizeof(WCHAR);
					full_path.Buffer = ExAllocatePoolWithTag(NonPagedPool, full_path.MaximumLength, BUFFER_TAG);
					RtlZeroMemory(full_path.Buffer, full_path.MaximumLength);
					RtlCopyUnicodeString(&full_path, &(nameInformation->Name));
					RtlAppendUnicodeToString(&full_path, L"\\");
					RtlAppendUnicodeStringToString(&full_path, &kObjectName);
				}
			}
		}
		else
			RtlInitUnicodeString(&full_path, kObjectName.Buffer);
		
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,sssssss,FileHandle->0x%08x,FileName->%wZ,DesiredAccess->0x%08x,CreateDisposition->%d,CreateOptions->%d,FileAttributes->%d,ShareAccess->%d", kFileHandle,&full_path, DesiredAccess, CreateDisposition, CreateOptions, FileAttributes, ShareAccess)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE,  L"0,%d,sssssss,FileHandle->0x%08x,FileName->%wZ,DesiredAccess->0x%08x,CreateDisposition->%d,CreateOptions->%d,FileAttributes->%d,ShareAccess->%d", statusCall, kFileHandle, &full_path, DesiredAccess, CreateDisposition, CreateOptions, FileAttributes, ShareAccess)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwCreateFile", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwCreateFile", L"0,-1,sssssss,FileHandle->ERROR,FileName->ERROR,DesiredAccess->ERROR,CreateDisposition->ERROR,CreateOptions->ERROR,FileAttributes->ERROR,ShareAccess->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwCreateFile", L"1,0,sssssss,FileHandle->ERROR,FileName->ERROR,DesiredAccess->ERROR,CreateDisposition->ERROR,CreateOptions->ERROR,FileAttributes->ERROR,ShareAccess->ERROR");
			break;
		}
		if(kObjectName.Buffer && kObjectName.Buffer != full_path.Buffer)
			ExFreePool(kObjectName.Buffer);
		if(parameter != NULL)
			ExFreePool(parameter);
		if(nameInformation != NULL)
			ExFreePool(nameInformation);
		if(full_path.Buffer)
			ExFreePool(full_path.Buffer);
	}
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs file reading.
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567072(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567072(v=vs.85).aspx
//	Process :
//		Gets the filename and logs it.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwReadFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset, PULONG Key)
{
	NTSTATUS statusCall, exceptionCode;
	ULONG currentProcessId, returnLength;
	USHORT log_lvl = LOG_ERROR;
	POBJECT_NAME_INFORMATION nameInformation = NULL;
	PWCHAR parameter = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWREADFILE)(oldZwReadFile))(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		
		nameInformation = ExAllocatePoolWithTag(NonPagedPool, MAXSIZE, BUFFER_TAG);
		if(nameInformation)
			ZwQueryObject(FileHandle, ObjectNameInformation, nameInformation, MAXSIZE, NULL);
		
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(nameInformation && parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,%d,ssss,FileHandle->0x%08x,FileName->%wZ,Buffer->0x%08x,Length->%d", statusCall, FileHandle, &(nameInformation->Name), Buffer, Length)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(nameInformation && parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE,  L"0,%d,ssss,FileHandle->0x%08x,FileName->%wZ,Buffer->0x%08x,Length->%d", statusCall, FileHandle, &(nameInformation->Name), Buffer, Length)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwReadFile", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwReadFile", L"0,-1,ssss,FileHandle->ERROR,FileName->ERROR,Buffer->ERROR,Length->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwReadFile", L"1,0,ssss,FileHandle->ERROR,FileName->ERROR,Buffer->ERROR,Length->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
		if(nameInformation != NULL)
			ExFreePool(nameInformation);
	}

	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs file modification.
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567121(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567121(v=vs.85).aspx
//	Process :
//		Gets the filename and logs it.
//	TODO :
//		- Log &Buffer, Length, ByteOffset
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwWriteFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset, PULONG Key)
{
	NTSTATUS statusCall;
	ULONG currentProcessId, returnLength;
	USHORT log_lvl = LOG_ERROR;
	POBJECT_NAME_INFORMATION nameInformation = NULL;
	PWCHAR parameter = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWWRITEFILE)(oldZwWriteFile))(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, Buffer, Length, ByteOffset, Key);

	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		nameInformation = ExAllocatePoolWithTag(NonPagedPool, MAXSIZE, BUFFER_TAG);
		if(nameInformation)
			ZwQueryObject(FileHandle, ObjectNameInformation, nameInformation, MAXSIZE, NULL);
		
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(nameInformation && parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,ssss,FileHandle->0x%08x,FileName->%wZ,Buffer->0x%08x,Length->%d", FileHandle, &(nameInformation->Name), Buffer, Length)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(nameInformation && parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE,  L"0,%d,ssss,FileHandle->0x%08x,FileName->%wZ,Buffer->0x%08x,Length->%d", statusCall, FileHandle, &(nameInformation->Name), Buffer, Length)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwWriteFile", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwWriteFile", L"0,-1,ssss,FileHandle->ERROR,FileName->ERROR,Buffer->ERROR,Length->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwWriteFile", L"1,0,ssss,FileHandle->ERROR,FileName->ERROR,Buffer->ERROR,Length->ERROR");
			break;
		}
		if(parameter != NULL)
			ExFreePool(parameter);
		if(nameInformation != NULL)
			ExFreePool(nameInformation);
	}

	return statusCall;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs file deletion.
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff566435(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff566435(v=vs.85).aspx
//	Process :
//		Copies the ObjectAttributes->ObjectName parameter then logs the file deletion.
//	TODO :
//		- Handle file deletion case: move the file in a special directory so it can be dumped.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwDeleteFile(POBJECT_ATTRIBUTES ObjectAttributes)
{
	NTSTATUS statusCall, exceptionCode;
	ULONG currentProcessId;
	USHORT log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	
	UNICODE_STRING kObjectName;
	kObjectName.Buffer = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWDELETEFILE)(oldZwDeleteFile))(ObjectAttributes);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		__try
		{
			if(ExGetPreviousMode() != KernelMode)
			{
				ProbeForRead(ObjectAttributes, sizeof(OBJECT_ATTRIBUTES), 1);
				ProbeForRead(ObjectAttributes->ObjectName, sizeof(UNICODE_STRING), 1);
				ProbeForRead(ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length, 1);
			}
			kObjectName.Length = ObjectAttributes->ObjectName->Length;
			kObjectName.MaximumLength = ObjectAttributes->ObjectName->Length;
			kObjectName.Buffer = ExAllocatePoolWithTag(NonPagedPool, kObjectName.MaximumLength, BUFFER_TAG);
			if(kObjectName.Buffer)
				RtlCopyUnicodeString(&kObjectName, ObjectAttributes->ObjectName);
			else
			{
				sendLogs(currentProcessId ,L"ZwDeleteFile", L"0,-1,s,FileName->ERROR");
				if(parameter)
					ExFreePool(parameter);
				return statusCall;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			exceptionCode = GetExceptionCode();
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,s,FileName->ERROR", exceptionCode)))
				sendLogs(currentProcessId, L"ZwDeleteFile", parameter);
			else 
				sendLogs(currentProcessId ,L"ZwDeleteFile", L"0,-1,s,FileName->ERROR");
			if(parameter)
				ExFreePool(parameter);
			if(kObjectName.Buffer)
				ExFreePool(kObjectName.Buffer);
			return statusCall;
		}
		
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,0,s,FileName->%wZ", &kObjectName)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE,  L"0,%d,s,FileName->%wZ", statusCall, &kObjectName)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwDeleteFile", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwDeleteFile", L"0,-1,s,FileName->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwDeleteFile", L"1,0,s,FileName->ERROR");
			break;
		}
		if(kObjectName.Buffer)
			ExFreePool(kObjectName.Buffer);
		if(parameter)
			ExFreePool(parameter);
	}
	
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs file deletion / rename.
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567096(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567096(v=vs.85).aspx
//	Process :
//		Copy the FileHandle parameter, then checks the FileInformationClass argument.
//		If FileDispositionInformation, the file may be deleted, the FileInformation->DeleteFile
//		parameter is copied and tested.
//		If FileRenameInformationrmation, the FileInformation->FileName parameter is copied along with the
//		FileInformation->RootDirectory parameter, then the call is logged.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwSetInformationFile(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass)
{
	NTSTATUS statusCall, exceptionCode;
	ULONG currentProcessId;
	USHORT log_lvl = LOG_ERROR;
	UNICODE_STRING full_path;
	POBJECT_NAME_INFORMATION originalNameInformation = NULL;
	PWCHAR parameter = NULL;
	PWCHAR newFileName = NULL;
	
	BOOLEAN kDeleteFile;
	ULONG kFileNameLength;
	PFILE_RENAME_INFORMATION kFileRenameInformation = NULL;
	PWCHAR kFileName = NULL;
	HANDLE kFileHandle, kRootDirectory;
	
	full_path.Buffer = NULL;
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWSETINFORMATIONFILE)(oldZwSetInformationFile))(FileHandle, IoStatusBlock, FileInformation, Length, FileInformationClass);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		__try 
		{
			if(ExGetPreviousMode() != KernelMode)
				ProbeForRead(FileHandle, sizeof(HANDLE), 1);
			kFileHandle = FileHandle;
		} 
		__except(EXCEPTION_EXECUTE_HANDLER)
		{
			exceptionCode = GetExceptionCode();
			if(FileInformationClass == FileDispositionInformation)
			{
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sss,FileHandle->ERROR,FileName->ERROR,FileInformationClass->ERROR", exceptionCode)))
					sendLogs(currentProcessId, L"ZwSetInformationFile (Delete)", parameter);
				else 
					sendLogs(currentProcessId, L"ZwSetInformationFile (Delete)", L"0,-1,sss,FileHandle->ERROR,FileName->ERROR,FileInformationClass->ERROR");
			}
			else if(FileInformationClass == FileRenameInformation)
			{
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,FileHandle->ERROR,OriginalName->ERROR,Renamed->ERROR,FileInformationClass->ERROR", exceptionCode)))
					sendLogs(currentProcessId, L"ZwSetInformationFile (Rename)", parameter);
				else 
					sendLogs(currentProcessId, L"ZwSetInformationFile (Rename)", L"0,-1,ssss,FileHandle->ERROR,OriginalName->ERROR,Renamed->ERROR,FileInformationClass->ERROR");
			}
			if(parameter)
				ExFreePool(parameter);
			return statusCall;
		}
		
		// CHANGE FILE DISPOSITION INFORMATION CASE
		if(FileInformationClass == FileDispositionInformation)
		{
			__try 
			{
				if(ExGetPreviousMode() != KernelMode)
					ProbeForRead(FileInformation, sizeof(FILE_DISPOSITION_INFORMATION), 1);
				kDeleteFile = ((PFILE_DISPOSITION_INFORMATION)FileInformation)->DeleteFile;
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				exceptionCode = GetExceptionCode();
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sss,FileHandle->ERROR,FileName->ERROR,FileInformationClass->ERROR", exceptionCode)))
					sendLogs(currentProcessId, L"ZwSetInformationFile (Delete)", parameter);
				else
					sendLogs(currentProcessId, L"ZwSetInformationFile (Delete)", L"0,-1,sss,FileHandle->ERROR,FileName->ERROR,FileInformationClass->ERROR");
				if(parameter)
					ExFreePool(parameter);
				return statusCall;
			}
			
			if(kDeleteFile == TRUE)
			{
				originalNameInformation = ExAllocatePoolWithTag(NonPagedPool, MAXSIZE, BUFFER_TAG);
				if(originalNameInformation && parameter)
					ZwQueryObject(kFileHandle, ObjectNameInformation, originalNameInformation, MAXSIZE, NULL);
				if(NT_SUCCESS(statusCall))
				{
					log_lvl = LOG_SUCCESS;
					if(parameter && originalNameInformation && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,%d,sss,FileHandle->0x%08x,FileName->%wZ,FileInformationClass->%d", statusCall, FileHandle, &(originalNameInformation->Name), FileInformationClass)))
						log_lvl = LOG_PARAM;
				}
				else
				{
					log_lvl = LOG_ERROR;
					if(parameter && originalNameInformation && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,sss,FileHandle->0x%08x,FileName->%wZ,FileInformationClass->%d", statusCall, FileHandle, &(originalNameInformation->Name), FileInformationClass)))
						log_lvl = LOG_PARAM;
				}
				switch(log_lvl)
				{
					case LOG_PARAM:
						sendLogs(currentProcessId, L"ZwSetInformationFile (Delete)", parameter);
					break;
					case LOG_SUCCESS:
						sendLogs(currentProcessId, L"ZwSetInformationFile (Delete)", L"1,0,sss,FileHandle->ERROR,FileName->ERROR,FileInformationClass->ERROR");
					break;
					default:
						sendLogs(currentProcessId, L"ZwSetInformationFile (Delete)", L"0,0,sss,FileHandle->ERROR,FileName->ERROR,FileInformationClass->ERROR");
					break;
				}
				if(originalNameInformation != NULL)
					ExFreePool(originalNameInformation);
			}
		}
		
		// RENAME FILE CASE
		if(FileInformationClass == FileRenameInformation)
		{
			__try 
			{
				if(ExGetPreviousMode() != KernelMode)
				{
					ProbeForRead(FileInformation, sizeof(FILE_RENAME_INFORMATION), 1);
					ProbeForRead(((PFILE_RENAME_INFORMATION)FileInformation)->FileName, ((PFILE_RENAME_INFORMATION)FileInformation)->FileNameLength, 1);
				}
				kFileRenameInformation = (PFILE_RENAME_INFORMATION)FileInformation;
				kRootDirectory = kFileRenameInformation->RootDirectory;
				kFileNameLength = kFileRenameInformation->FileNameLength;
				kFileName = ExAllocatePoolWithTag(NonPagedPool, kFileNameLength + sizeof(WCHAR), BUFFER_TAG);
				if(!kFileName)
				{
					sendLogs(currentProcessId, L"ZwSetInformationFile (Rename)", L"0,-1,ssss,FileHandle->ERROR,OriginalName->ERROR,Renamed->ERROR,FileInformationClass->ERROR");
					if(parameter)
						ExFreePool(parameter);
					return statusCall;
				}
				RtlZeroMemory(kFileName, kFileNameLength + sizeof(WCHAR));
				RtlCopyMemory(kFileName, kFileRenameInformation->FileName, kFileNameLength);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				exceptionCode = GetExceptionCode();
				if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,FileHandle->ERROR,OriginalName->ERROR,Renamed->ERROR,FileInformationClass->ERROR", exceptionCode)))
					sendLogs(currentProcessId, L"ZwSetInformationFile (Rename)", parameter);
				else
					sendLogs(currentProcessId, L"ZwSetInformationFile (Rename)", L"0,-1,ssss,FileHandle->ERROR,OriginalName->ERROR,Renamed->ERROR,FileInformationClass->ERROR");
				if(parameter)
					ExFreePool(parameter);
				if(kFileName)
					ExFreePool(kFileName);
				return statusCall;
			}
			
			if(kRootDirectory)	// handle the not null RootDirectory case
			{
				// allocate both name information struct and unicode string buffer
				originalNameInformation = ExAllocatePoolWithTag(NonPagedPool, MAXSIZE, BUFFER_TAG);
				if(originalNameInformation)
				{
					if(NT_SUCCESS(ZwQueryObject(kRootDirectory, ObjectNameInformation, originalNameInformation, MAXSIZE, NULL)) && kFileNameLength < 0xFFF0)
					{
						full_path.MaximumLength = originalNameInformation->Name.Length + (USHORT)kFileNameLength + 2 + sizeof(WCHAR);
						full_path.Buffer = ExAllocatePoolWithTag(NonPagedPool, full_path.MaximumLength, BUFFER_TAG);
						RtlZeroMemory(full_path.Buffer, full_path.MaximumLength);
						RtlCopyUnicodeString(&full_path, &(originalNameInformation->Name));
						RtlAppendUnicodeToString(&full_path, L"\\");
						RtlAppendUnicodeToString(&full_path, kFileName);
					}
				}
				else
					RtlInitUnicodeString(&full_path, kFileName);
			}
			else
				RtlInitUnicodeString(&full_path, kFileName);
			
			originalNameInformation = ExAllocatePoolWithTag(NonPagedPool, MAXSIZE, BUFFER_TAG);
			if(originalNameInformation && parameter)
				ZwQueryObject(kFileHandle, ObjectNameInformation, originalNameInformation, MAXSIZE, NULL);
			
			if(NT_SUCCESS(statusCall))
			{
				log_lvl = LOG_SUCCESS;
				if(parameter && originalNameInformation && kFileName)
					if(NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,ssss,FileHandle->0x%08x,OriginalName->%wZ,Renamed->%wZ,FileInformationClass->%d", FileHandle, &(originalNameInformation->Name), &full_path, FileInformationClass)))
						log_lvl = LOG_PARAM;
			}
			else
			{
				log_lvl = LOG_ERROR;
				if(parameter && originalNameInformation && kFileName)
					if(NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,FileHandle->0x%08x,,OriginalName->%wZ,Renamed->%wZ,FileInformationClass->%d", statusCall, FileHandle, &(originalNameInformation->Name), &full_path, FileInformationClass)))
						log_lvl = LOG_PARAM;
			}
			
			if(full_path.Buffer && full_path.Buffer != kFileName)
				ExFreePool(full_path.Buffer);
			if(kFileName)
				ExFreePool(kFileName);
			if(originalNameInformation)
				ExFreePool(originalNameInformation);
			
			switch(log_lvl)
			{
				case LOG_PARAM:
					sendLogs(currentProcessId, L"ZwSetInformationFile (Rename)", parameter);
				break;
				case LOG_SUCCESS:
					sendLogs(currentProcessId, L"ZwSetInformationFile (Rename)", L"1,0,ssss,FileHandle->ERROR,OriginalName->ERROR,Renamed->ERROR,FileInformationClass->ERROR");
				break;
				default:
					sendLogs(currentProcessId, L"ZwSetInformationFile (Rename)", L"0,-1,ssss,FileHandle->ERROR,OriginalName->ERROR,Renamed->ERROR,FileInformationClass->ERROR");
				break;
			}
		}
		if(parameter)
			ExFreePool(parameter);
	}
	
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs file information access.
//	Parameters :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567052(v=vs.85).aspx
//	Return value :
//		See http://msdn.microsoft.com/en-us/library/windows/hardware/ff567052(v=vs.85).aspx
//	Process :
//		Logs file handle and fileinformationclass.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwQueryInformationFile(HANDLE FileHandle, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass)
{
	NTSTATUS statusCall, exceptionCode;
	DWORD currentProcessId;
	USHORT log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWQUERYINFORMATIONFILE)(oldZwQueryInformationFile))(FileHandle, IoStatusBlock, FileInformation, Length, FileInformationClass);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
	
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,ss,FileHandle->0x%08x,FileInformationClass->%d", FileHandle, FileInformationClass)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE,  L"0,%d,ss,FileHandle->0x%08x,FileInformationClass->%d", statusCall, FileHandle, FileInformationClass)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwQueryInformationFile", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwQueryInformationFile", L"0,-1,ss,FileHandle->ERROR,FileInformationClass->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwQueryInformationFile", L"1,0,ss,FileHandle->ERROR,FileInformationClass->ERROR");
			break;
		}
		if(parameter)
			ExFreePool(parameter);
	}

	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Logs mutex creation
//	Parameters :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/NT%20Objects/Mutant/NtCreateMutant.html
//	Return value :
//		See http://undocumented.ntinternals.net/UserMode/Undocumented%20Functions/NT%20Objects/Mutant/NtCreateMutant.html
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS newZwCreateMutant(PHANDLE MutantHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, BOOLEAN InitialOwner)
{
	NTSTATUS statusCall, exceptionCode;
	DWORD currentProcessId;
	HANDLE kMutantHandle;
	USHORT log_lvl = LOG_ERROR;
	PWCHAR parameter = NULL;
	UNICODE_STRING kObjectName;
	kObjectName.Buffer = NULL;
	
	currentProcessId = (ULONG)PsGetCurrentProcessId();
	statusCall = ((ZWCREATEMUTANT)(oldZwCreateMutant))(MutantHandle, DesiredAccess, ObjectAttributes, InitialOwner);
	
	if(isProcessMonitoredByPid(currentProcessId))
	{
		parameter = ExAllocatePoolWithTag(NonPagedPool, (MAXSIZE+1)*sizeof(WCHAR), PROC_POOL_TAG);
		
		__try
		{
			if(ExGetPreviousMode() != KernelMode)
			{
				ProbeForRead(MutantHandle, sizeof(HANDLE), 1);
				ProbeForRead(ObjectAttributes, sizeof(OBJECT_ATTRIBUTES), 1);
				ProbeForRead(ObjectAttributes->ObjectName, sizeof(UNICODE_STRING), 1);
				ProbeForRead(ObjectAttributes->ObjectName->Buffer, ObjectAttributes->ObjectName->Length, 1);
			}
			kMutantHandle = *MutantHandle;
			kObjectName.Length = ObjectAttributes->ObjectName->Length;
			kObjectName.MaximumLength = ObjectAttributes->ObjectName->Length;
			kObjectName.Buffer = ExAllocatePoolWithTag(NonPagedPool, kObjectName.MaximumLength, BUFFER_TAG);
			if(kObjectName.Buffer)
				RtlCopyUnicodeString(&kObjectName, ObjectAttributes->ObjectName);
			else
			{
				sendLogs(currentProcessId ,L"ZwCreateMutant", L"0,-1,ssss,MutantHandle->ERROR,DesiredAccess->ERROR,ObjectAttributes->ERROR,InitialOwner->ERROR");
				if(parameter)
					ExFreePool(parameter);
				return statusCall;
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			exceptionCode = GetExceptionCode();
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"0,%d,ssss,MutantHandle->ERROR,DesiredAccess->ERROR,ObjectAttributes->ERROR,InitialOwner->ERROR", exceptionCode)))
				sendLogs(currentProcessId, L"ZwCreateMutant", parameter);
			else 
				sendLogs(currentProcessId ,L"ZwCreateMutant", L"0,-1,ssss,MutantHandle->ERROR,DesiredAccess->ERROR,ObjectAttributes->ERROR,InitialOwner->ERROR");
			if(parameter)
				ExFreePool(parameter);
			if(kObjectName.Buffer)
				ExFreePool(kObjectName.Buffer);
			return statusCall;
		}
	
		if(NT_SUCCESS(statusCall))
		{
			log_lvl = LOG_SUCCESS;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE, L"1,0,ssss,MutantHandle->0x%08x,DesiredAccess->0x%08x,MutexName->%wZ,InitialOwner->%d", kMutantHandle, DesiredAccess, &kObjectName, InitialOwner)))
				log_lvl = LOG_PARAM;
		}
		else
		{
			log_lvl = LOG_ERROR;
			if(parameter && NT_SUCCESS(RtlStringCchPrintfW(parameter, MAXSIZE,  L"0,%d,ssss,MutantHandle->0x%08x,DesiredAccess->0x%08x,MutexName->%wZ,InitialOwner->%d", statusCall, kMutantHandle, DesiredAccess, &kObjectName, InitialOwner)))
				log_lvl = LOG_PARAM;
		}
		
		switch(log_lvl)
		{
			case LOG_PARAM:
				sendLogs(currentProcessId, L"ZwCreateMutant", parameter);
			break;
			case LOG_SUCCESS:
				sendLogs(currentProcessId, L"ZwCreateMutant", L"0,-1,ssss,MutantHandle->ERROR,DesiredAccess->ERROR,ObjectAttributes->ERROR,InitialOwner->ERROR");
			break;
			default:
				sendLogs(currentProcessId, L"ZwCreateMutant", L"1,0,ssss,MutantHandle->ERROR,DesiredAccess->ERROR,ObjectAttributes->ERROR,InitialOwner->ERROR");
			break;
		}
		if(kObjectName.Buffer)
			ExFreePool(kObjectName.Buffer);
		if(parameter)
			ExFreePool(parameter);
	}
	return statusCall;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Unsets WP bit of CR0 register (allows writing into SSDT).
//	Parameters :
//		None
//	Return value :
//		None
//	Notes :
//		http://en.wikipedia.org/wiki/Control_register#CR0
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID disable_cr0()
{
	__asm
	{
		push eax
		mov eax, CR0
		and eax, 0FFFEFFFFh
		mov CR0, eax
		pop eax
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		Sets WP bit of CR0 register.
//	Parameters :
//		None
//	Return value :
//		None
//	Notes :
//		http://en.wikipedia.org/wiki/Control_register#CR0
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
VOID enable_cr0()
{
	__asm
	{
		push eax
		mov eax, CR0
		or eax, NOT 0FFFEFFFFh
		mov CR0, eax
		pop eax
	}
}
