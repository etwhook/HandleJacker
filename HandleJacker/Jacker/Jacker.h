#pragma once
#include <iostream>
#include "ntapi.h"

namespace Jacker {

	PSYSTEM_HANDLE_INFORMATION GetSysHandleInfo();
	HANDLE DupHandle(SYSTEM_HANDLE_TABLE_ENTRY_INFO handleableEntryInfo);
	POBJECT_NAME_INFORMATION GetObjNameInfo(HANDLE duppedHandle);
	POBJECT_TYPE_INFORMATION GetObjTypeInfo(HANDLE duppedHandle);
	POBJECT_BASIC_INFORMATION GetObjBasicInfo(HANDLE duppedHandle);
	PUNICODE_STRING GetProcessFullImageName(HANDLE hProcess);
	LPWSTR GetProcessNameFromId(DWORD procId);
	
}