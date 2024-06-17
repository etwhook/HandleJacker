#include <iostream>
#include "./Jacker/Jacker.h"
int main()
{
	Sleep(3500);
    auto handleInfo = Jacker::GetSysHandleInfo();

	if (!handleInfo) {
		return -1;
	}

	for (size_t i = 0; i < handleInfo->NumberOfHandles; i++)
	{
		
		auto handleTableEntry = handleInfo->Handles[i];
		HANDLE hijackedHandle = Jacker::DupHandle(handleTableEntry);
		
		if (hijackedHandle) {
			
			auto objTypeInfo = Jacker::GetObjTypeInfo(hijackedHandle);
			
			if (!wcscmp(objTypeInfo->TypeName.Buffer, L"Process")) {

				auto objBasicInfo = Jacker::GetObjBasicInfo(hijackedHandle);
				auto processId = GetProcessId(hijackedHandle);

				if (processId == GetCurrentProcessId()) {
					ACCESS_MASK handleAM = objBasicInfo->GrantedAccess;
					DWORD targetProcessId = (DWORD)handleTableEntry.UniqueProcessId;
					LPWSTR targetProcessName = Jacker::GetProcessNameFromId(targetProcessId);
					wprintf(L"[!] %ls Has an Opened Handle Of The Current Process, Acess Mask: ( %x )!\n", targetProcessName, handleAM);
					
					// simple example here, you can also add access mask checks to see if the handle has VM_READ or VM_WRITE.
					
					if (!wcscmp(targetProcessName, L"ProcessHacker.exe")) {
						HANDLE targetProcess = OpenProcess(PROCESS_TERMINATE, FALSE, targetProcessId);

						if (NT_SUCCESS(NtTerminateProcess(targetProcess, 0))) {
							printf("[+] Terminated Process Hacker!\n");
						}
					}

				}
			}
			
		}
	}
}

