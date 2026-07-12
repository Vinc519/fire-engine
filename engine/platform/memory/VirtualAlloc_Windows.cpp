#include "VirtualAlloc.h"

#include "core/debug/Logger.h"

//#include <memoryapi.h>
//#include <sysinfoapi.h>
//#include <errhandlingapi.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void* VirtualMemory_Reserve(u64 size)
{
	return VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
}

b8 VirtualMemory_Commit(void* ptr, u64 size)
{
	void* result = VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE);
	return result != nullptr;
}

b8 VirtualMemory_Decommit(void* ptr, u64 size)
{
	u8 result = VirtualFree(ptr, size, MEM_DECOMMIT);
	if (!result)
		FE_LOG_WARN("VirtualFree (decommit) failed with error code: %lu", GetLastError());
	return result != 0;
}

void VirtualMemory_Release(void* ptr, u64 size)
{
	u8 result = VirtualFree(ptr, 0, MEM_RELEASE);
	if (!result)
		FE_LOG_WARN("VirtualFree failed with error code: %d", GetLastError());
}

u64 VirtualMemory_GetPageSize()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	return static_cast<u64>(sysInfo.dwPageSize);
}