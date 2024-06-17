
# HandleJacker

Simple Handle Hijacking / Manipulation Library.

## Example
I provide an example of usage in **HandleJacker.cpp** where the library is used to enumerate all system handles, Get their type information and if it is a process handle then move on, Then proceeds to fetch its **ObjectBasicInformation** to get the granted access for the handle (Which can be used to check if the handle has certain permissions like **VM_READ**), After that it will get the process name and check it against **ProcessHacker.exe**, Then terminates the process if the match was successful.

```cpp
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
```

## Utilizing This In An Anti Tamper Software

This can be very helpful to stop cheaters from opening a handle to your process, You can either terminate the current process if a handle was opened from an unsigned process with specific permissions, Or just close the handle that was opened in the first place.

## Stripping Permissions?
As far as i know, Stripping permissions from a process handle (usermode) **is NOT** possible, I have tried using **NtSetInformationObject** with **ObjectBasicInformation** with a new **GrantedAccess** value, But i got an error saying that **ObjectBasicInformation** was **NOT** a valid information class for this operation. 
