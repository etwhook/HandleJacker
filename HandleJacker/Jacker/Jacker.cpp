#include "Jacker.h"
#include<Tlhelp32.h>

PSYSTEM_HANDLE_INFORMATION Jacker::GetSysHandleInfo() {
	ULONG handleInfoSize = 0;
	NTSTATUS qInfoStatus = STATUS_INFO_LENGTH_MISMATCH;

	PVOID sysHandleInfo = NULL;

	while (qInfoStatus == STATUS_INFO_LENGTH_MISMATCH) {
		qInfoStatus = NtQuerySystemInformation(SystemHandleInformation, sysHandleInfo, handleInfoSize, &handleInfoSize);
		sysHandleInfo = realloc(sysHandleInfo, handleInfoSize);
	}

	return (PSYSTEM_HANDLE_INFORMATION)sysHandleInfo;	
}

HANDLE Jacker::DupHandle(SYSTEM_HANDLE_TABLE_ENTRY_INFO handleTableEntryInfo) {
	DWORD ownerProcessId = (DWORD)handleTableEntryInfo.UniqueProcessId;
	HANDLE handleValue = (HANDLE)handleTableEntryInfo.HandleValue;
	ACCESS_MASK handleAccess = (ACCESS_MASK)handleTableEntryInfo.GrantedAccess;
	ULONG handleAttr = (ULONG)handleTableEntryInfo.HandleAttributes;
	HANDLE dupHandle = 0;

	HANDLE ownerProcess = OpenProcess(PROCESS_DUP_HANDLE, FALSE, ownerProcessId);

	if (NT_SUCCESS(NtDuplicateObject(ownerProcess, handleValue, NtCurrentProcess, &dupHandle, handleAccess, handleAttr, 0))) {

		return dupHandle;
	}
	else {
		return NULL;
	}
}

POBJECT_NAME_INFORMATION Jacker::GetObjNameInfo(HANDLE duppedHandle) {
	ULONG objInfoLen = 0;
	NTSTATUS qInfoStatus = STATUS_INFO_LENGTH_MISMATCH;
	PVOID objInfo = NULL;

	while (qInfoStatus == STATUS_INFO_LENGTH_MISMATCH) {
		qInfoStatus = NtQueryObject(duppedHandle, ObjectNameInformation, objInfo, objInfoLen, &objInfoLen);
		objInfo = realloc(objInfo, objInfoLen);
	}

	return (POBJECT_NAME_INFORMATION)objInfo;
}

POBJECT_TYPE_INFORMATION Jacker::GetObjTypeInfo(HANDLE duppedHandle) {

	ULONG objInfoLen = 0;
	NTSTATUS qInfoStatus = STATUS_INFO_LENGTH_MISMATCH;
	PVOID objInfo = NULL;
	
	while (qInfoStatus == STATUS_INFO_LENGTH_MISMATCH) {
		qInfoStatus = NtQueryObject(duppedHandle, ObjectTypeInformation, objInfo, objInfoLen, &objInfoLen);
		objInfo = realloc(objInfo, objInfoLen);
	}
	
	return (POBJECT_TYPE_INFORMATION)objInfo;
}

POBJECT_BASIC_INFORMATION Jacker::GetObjBasicInfo(HANDLE duppedHandle) {

	ULONG objInfoLen = sizeof(OBJECT_BASIC_INFORMATION);
	PVOID objInfo = malloc(objInfoLen);
	NtQueryObject(duppedHandle, ObjectBasicInformation, objInfo, objInfoLen, &objInfoLen);

	return (POBJECT_BASIC_INFORMATION)objInfo;
}

PUNICODE_STRING Jacker::GetProcessFullImageName(HANDLE hProcess) {
	ULONG infoLen = 0;
	NTSTATUS qInfoStatus = STATUS_INFO_LENGTH_MISMATCH;
	PVOID unicodeStrBuffer = NULL;

	while (qInfoStatus == STATUS_INFO_LENGTH_MISMATCH) {
		qInfoStatus = NtQueryInformationProcess(hProcess, ProcessImageFileName, unicodeStrBuffer, infoLen, &infoLen);
		unicodeStrBuffer = realloc(unicodeStrBuffer, infoLen);
	}
	
	return (PUNICODE_STRING)unicodeStrBuffer;
}

LPWSTR Jacker::GetProcessNameFromId(DWORD procId) {
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	while (Process32Next(snapShot, &pe32)) {
		if (pe32.th32ProcessID == procId) {
			CloseHandle(snapShot);
			return (LPWSTR)pe32.szExeFile;
		}
	}

	CloseHandle(snapShot);
	return NULL;
}